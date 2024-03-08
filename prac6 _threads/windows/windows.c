#include <unistd.h>
#include <windows.h>

#include "mem.h"
#include "scan.h"


struct ChildArg {
    int iStart;
    int iEnd;
};


CRITICAL_SECTION cs;
DOUBLE totalSum = 0;
LPSTR *nums;
HANDLE StdErr;


DWORD WINAPI process(LPVOID arguments) {
    struct ChildArg *args = (struct ChildArg *) arguments;
    DOUBLE localSum = 0;

    for (int i = args->iStart; i < args->iEnd; i++) {
        LPSTR endPtr;
        DOUBLE num = strtod(nums[i], &endPtr);
        if ((num == HUGE_VAL || num == 0) && errno == ERANGE) {
            _perror(StdErr, "Error converting to double");
            ExitThread(1);
        }
        localSum += sqrt(num);
    }

    EnterCriticalSection(&cs);
    totalSum += localSum;
    LeaveCriticalSection(&cs);

    // Sleep(20000);
}


int main(int argc, char **argv) {
    StdErr = GetStdHandle(STD_ERROR_HANDLE);
    LPSTR filename;
    int N;

    if (argc != 3) {
        RaiseError("Please specify file to read and number of processes accordingly (e.g ./parent.exe <filename> <N>)");
        ExitProcess(1);
    } else {
        filename = argv[1];
        N = atoi(argv[2]);
        if (N <= 0) {
            RaiseError("Number of process must be a positive integer");
            ExitProcess(1);
        }
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        _perror(StdErr, "Error opening file");
        ExitProcess(1);
    }
    
    nums = (LPSTR*)malloc(MAX_NUMS * sizeof(LPSTR));
    for (int i = 0; i < MAX_NUMS; i++) {
        nums[i] = (LPSTR)malloc(16 * sizeof(CHAR));
    }

    int totalNums = 0;
    while(!feof(file)) {
        CHAR numStr[INT_LEN];
        if (scanInt(file, numStr) < 0) {
            fclose(file);
            freeNums(nums);
            ExitProcess(1);
        }
        strcpy(nums[totalNums++], numStr);

        if (totalNums == MAX_NUMS) {
            RaiseError("Too many numbers in file");
            fclose(file);
            freeNums(nums);
            ExitProcess(1);
        }
    }
    fclose(file);

    if (totalNums < 2) {
        RaiseError("There must be 2 or more numbers in a file");
        freeNums(nums);
        ExitProcess(1);
    }
    if (N > totalNums / 2) {
        RaiseWarn("There are too many child processes, decreasing...");
        N = totalNums / 2;
    }

    int nEach = totalNums / N;
    int nLast = totalNums - (N - 1) * nEach;

    InitializeCriticalSection(&cs);
    int iStart = 0;

    HANDLE *threads = (HANDLE *)malloc(N * sizeof(HANDLE));
    if (threads == NULL) {
        _perror(StdErr, "Failed to malloc for threads");
        freeNums(nums);
        ExitProcess(1);
    }
    struct ChildArg *childArgs = (struct ChildArg *)malloc(N * sizeof(struct ChildArg));
    if (childArgs == NULL) {
        _perror(StdErr, "Failed to malloc for child args");
        freeNums(nums);
        free(threads);
        ExitProcess(1);
    }

    for (int i = 0; i < N; i++) {
        int nWrite = (i == N - 1) ? nLast : nEach;

        childArgs[i].iStart = iStart;
        childArgs[i].iEnd = iStart + nWrite;

        iStart = childArgs[i].iEnd;
        DWORD threadID = 0;
        threads[i] = CreateThread(NULL, 0, process, (LPVOID)&childArgs[i], CREATE_NEW, &threadID);
        if (threads[i] == NULL) {
            _perror(StdErr, "Failed to create new thread");
            freeNums(nums);
            free(threads);
            free(childArgs);
            ExitProcess(1);
        }
    }

    DWORD res = WaitForMultipleObjects((DWORD)N, threads, TRUE, INFINITE);
    if (res == WAIT_FAILED) {
        _perror(StdErr, "Failed to wait for all running thread");
        freeNums(nums);
        free(threads);
        free(childArgs);
        ExitProcess(1);
    }
    
    DeleteCriticalSection(&cs);
    freeNums(nums);
    free(threads);
    free(childArgs);

    printf("The final sum of square roots is: %lf\n", totalSum);
    ExitProcess(0);
}