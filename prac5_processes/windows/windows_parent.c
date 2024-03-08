#include <unistd.h>

#include "mem.h"
#include "scan.h"


HANDLE startSubprocess(int i, char *n, HANDLE *StdErr) {
    char args[MAX_FILENAME];
    sprintf(args, "files/file%d.txt %s", i + 1, n);

    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    GetStartupInfo(&si);

    BOOL res = CreateProcess("child.exe", args, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    if (!res) {
        _perror(*StdErr, "Error creating process");
        ExitProcess(1);
    }

    return pi.hProcess;
}


int main(int argc, char **argv) {
    HANDLE StdErr = GetStdHandle(STD_ERROR_HANDLE);
    char* filename;
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
    
    char **nums = (char **)malloc(MAX_NUMS * sizeof(char *));
    for (int i = 0; i < MAX_NUMS; i++) {
        nums[i] = (char *)malloc(16 * sizeof(char));
    }

    int totalNums = 0;
    while(!feof(file)) {
        char numStr[INT_LEN];
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

    for (int i = 0; i < N; i++) {
        int nWrite = (i == N - 1) ? nLast : nEach;
        char fname[MAX_FILENAME];
        sprintf(fname, "files/file%d.txt", i + 1);

        FILE *childFile = fopen(fname, "w");
        if (childFile == NULL) {
            _perror(StdErr, "Error opening file");
            freeNums(nums);
            fclose(childFile);
            ExitProcess(1);
        }

        for (int j = 0; j < nWrite; j++) {
            fwrite(nums[i * nEach + j], 1, strlen(nums[i * nEach + j]), childFile);
            if (j != nWrite - 1) {
                fwrite(" ", 1, 1, childFile);
            }
        }
        fclose(childFile);
    }

    freeNums(nums);
    HANDLE *handlers = (HANDLE *)malloc(N * sizeof(HANDLE));

    char nEachStr[INT_LEN], nLastStr[INT_LEN];
    sprintf(nEachStr, "%d", nEach);
    sprintf(nLastStr, "%d", nLast);

    for (int i = 0; i < N; i++) {
        handlers[i] = startSubprocess(i, (i == N - 1) ? nLastStr: nEachStr, &StdErr);
    }

    DWORD res = WaitForMultipleObjects((DWORD)N, handlers, TRUE, INFINITE);
    if (res == WAIT_FAILED) {
        _perror(StdErr, "Error waiting for all subprocesses");
        free(handlers);
        ExitProcess(1);
    }

    double sum = 0;
    for (int i = 0; i < N; i++) {
        char fname[MAX_FILENAME];
        sprintf(fname, "files/file%d.txt", i + 1);

        FILE *childFile = fopen(fname, "r");
        if (childFile == NULL) {
            _perror(StdErr, "Error opening file: ");
            fclose(childFile);
            free(handlers);
            ExitProcess(1);
        }
        while (fgetc(childFile) != '\n' && !feof(childFile)) {}
        
        if (feof(childFile)) {
            RaiseError("No new data or newline character found in file");
            free(handlers);
            fclose(childFile);
        }

        double num = 0;
        while(!feof(childFile)) {
            char numStr[FLOAT_LEN];
            if (scanDouble(childFile, numStr) < 0) {
                fclose(childFile);
                free(handlers);
                ExitProcess(1);
            }
            char *endPtr;
            num = strtod(numStr, &endPtr);
            if ((num == HUGE_VAL || num == 0) && errno == ERANGE) {
                _perror(StdErr, "Error converting to double:");
                fclose(childFile);
                free(handlers);
                ExitProcess(1);
            }
        }
        sum += num;
        fclose(childFile);
    }

    printf("The final sum of square roots is: %lf\n", sum);
    ExitProcess(0);
}