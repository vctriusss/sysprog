#pragma once

#include <stdio.h>
#include <windows.h>


VOID RaiseError(char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
}


VOID RaiseWarn(char *msg) {
    fprintf(stdout, "[WARN] %s\n", msg);
}


VOID _print_many(HANDLE stream, LPSTR messages[], size_t size) {
    DWORD dwBytesWritten = 0;
    for (size_t i = 0; i < size; i++) {
        WriteConsole(stream, messages[i], strlen(messages[i]), &dwBytesWritten, NULL);
    }
}


VOID _perror(HANDLE hStdErr, LPSTR message) {
    LPSTR errMsg;
    DWORD dwMessageLen = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        (LPSTR)&errMsg,
        0,
        NULL);

    LPSTR msgs[3] = {message, ": ", errMsg};
    _print_many(hStdErr, msgs, 3);
}
