#include <windows.h>

#include "mem.h"
#include "scan.h"


INT parent_shm(INT N, INT totalNums, FILE* file) {
    INT nEach = totalNums / N;
    INT nLast = totalNums - (N - 1) * nEach;

    HANDLE StdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE StdErr = GetStdHandle(STD_ERROR_HANDLE);

    HANDLE nums_fd = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, totalNums * sizeof(INT), "nums");
    if (nums_fd == NULL) {
        _perror(StdErr, "Failed to open shared memory");
        return 1;
    }
    INT *nums_mem = (INT*)MapViewOfFile(nums_fd, FILE_MAP_ALL_ACCESS, 0, 0, totalNums * sizeof(INT));
    if (nums_mem == NULL) {
        _perror(StdErr, "Failed to map memory");
        return 1;
    }

    HANDLE res_fd = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1 * sizeof(double), "res");
    if (res_fd == NULL) {
        _perror(StdErr, "Failed to open shared memory");
        return 1;
    }
    double *res_mem = (double*)MapViewOfFile(res_fd, FILE_MAP_ALL_ACCESS, 0, 0, 1 * sizeof(double));
    if (res_mem == NULL) {
        _perror(StdErr, "Failed to map memory");
        return 1;
    }

    HANDLE mu = CreateMutex(NULL, FALSE, "mu");
    if (mu == NULL) {
        _perror(StdErr, "Failed to create mutex");
        return 1;
    }

    HANDLE *handlers = (HANDLE *)malloc(N * sizeof(HANDLE));
    if (handlers == NULL) {
        _perror(StdErr, "Failed to malloc for handlers");
        return 1;
    }

    CHAR numStr[INT_LEN];
    INT num;
    INT *ptr = nums_mem;

    for (INT i = 0; i < N; i++) {
        INT nWrite = (i == N - 1) ? nLast : nEach;
        CHAR args[3 * INT_LEN + 3];
        sprintf(args, "%d %d %d", i * nEach, nWrite, totalNums);

        for (INT j = 0; j < nWrite; j++) {
            scanInt(file, numStr);
            num = atoi(numStr);
            memcpy(ptr++, &num, sizeof(INT));
        }

        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        GetStartupInfo(&si);

        BOOL res = CreateProcess("child_shm.exe", args, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        if (!res) {
            _perror(StdErr, "Error creating process");
            return 1;
        }
        handlers[i] = pi.hProcess;
    }
    DWORD res = WaitForMultipleObjects((DWORD)N, handlers, TRUE, INFINITE);
    if (res == WAIT_FAILED) {
        _perror(StdErr, "Error waiting for all subprocesses");
        return 1;
    }
    free(handlers);
    CloseHandle(mu);
    
    printf("The final sum of square roots is: %lf\n", *res_mem);

    if (!UnmapViewOfFile(nums_mem)) {
        _perror(StdErr, "Failed to unmap file view");
        return 1;
    }
    if (!UnmapViewOfFile(res_mem)) {
        _perror(StdErr, "Failed to unmap file view");
        return 1;
    }
    CloseHandle(nums_fd);
    CloseHandle(res_fd);
    
    return 0;
}