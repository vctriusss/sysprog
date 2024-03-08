#include <unistd.h>
#include <fcntl.h>

#include "mem.h"
#include "scan.h"


VOID InitializeSecurityAttr(LPSECURITY_ATTRIBUTES attr, SECURITY_DESCRIPTOR * sd) {
    attr->nLength = sizeof(SECURITY_ATTRIBUTES);
    attr->bInheritHandle = TRUE; 
    InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION);
    attr->lpSecurityDescriptor = sd;
}


INT parent_pipes(INT N, INT totalNums, FILE *file) {
    INT nEach = totalNums / N;
    INT nLast = totalNums - (N - 1) * nEach;

    HANDLE StdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE StdErr = GetStdHandle(STD_ERROR_HANDLE);

    HANDLE *handlers = (HANDLE *)malloc(N * sizeof(HANDLE));
    if (handlers == NULL) {
        _perror(StdErr, "Failed to malloc for handlers");
        return 1;
    }
    HANDLE *parent_read_pipes = (HANDLE *)malloc(N * sizeof(INT));
    if (parent_read_pipes == NULL) {
        _perror(StdErr, "Failed to malloc for handlers");
        return 1;
    }

    SECURITY_ATTRIBUTES attr;
    SECURITY_DESCRIPTOR sd;
    InitializeSecurityAttr(&attr, &sd);

    for (INT i = 0; i < N; i++) {
        INT nWrite = (i == N - 1) ? nLast : nEach;
        CHAR nWriteStr[INT_LEN];
        sprintf(nWriteStr, "%d", nWrite);

        HANDLE parent_write_pipe, child_read_pipe;
        if (!CreatePipe(&child_read_pipe, &parent_write_pipe , &attr, 0)) {
            _perror(StdErr, "Failed to create pipe");
            return 1;
        }

        HANDLE child_write_pipe, parent_read_pipe;
        if (!CreatePipe(&parent_read_pipe, &child_write_pipe , &attr, 0)) {
            _perror(StdErr, "Failed to create pipe");
            return 1;
        }
        parent_read_pipes[i] = parent_read_pipe;

        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        GetStartupInfo(&si);

        SetStdHandle(STD_INPUT_HANDLE, child_read_pipe);
        SetStdHandle(STD_OUTPUT_HANDLE, child_write_pipe); 

        BOOL res = CreateProcess("child_pipes.exe", nWriteStr, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        if (!res) {
            _perror(StdErr, "Error creating process");
            ExitProcess(1);
        }
        handlers[i] = pi.hProcess;

        CloseHandle(child_read_pipe);
        CloseHandle(child_write_pipe);
    
        SetStdHandle(STD_INPUT_HANDLE, StdIn);
        SetStdHandle(STD_OUTPUT_HANDLE, StdOut); 

        DWORD dwBytes = 1;
        // writing to pipes
        for (INT j = 0; j < nWrite; j++) {
            CHAR numStr[INT_LEN];
            scanInt(file, numStr);

            if (!WriteFile(parent_write_pipe, numStr, strlen(numStr), &dwBytes, NULL)) {  
                _perror(StdErr, "Failed to write to pipe from parent");
                return 1;
            }
            if (!WriteFile(parent_write_pipe, " ", 1, &dwBytes, NULL)) {  
                _perror(StdErr, "Failed to write to pipe from parent");
                return 1;
            }
        }
        CloseHandle(parent_write_pipe);
    }
    fclose(file);

    DWORD res = WaitForMultipleObjects((DWORD)N, handlers, TRUE, INFINITE);
    if (res == WAIT_FAILED) {
        _perror(StdErr, "Error waiting for all subprocesses");
        ExitProcess(1);
    }
    free(handlers);

    double sum = 0;
    for (INT i = 0; i < N; i++) {
        CHAR doubleStr[FLOAT_LEN];
        if (scanDouble(parent_read_pipes[i], doubleStr) < 0) {
            RaiseError("Failed to scan double from parent");
            return 1;
        }
        sum += strtod(doubleStr, NULL);
        CloseHandle(parent_read_pipes[i]);
    }

    free(parent_read_pipes);
    printf("The final sum of square roots is: %lf\n", sum);
    return 0;
}