#include <windows.h>
#include <stdio.h>


#define BUFF_SIZE 1024


VOID _print(HANDLE stream, LPSTR message) {
    DWORD dwBytesWritten = 0;
    WriteConsole(stream, message, strlen(message), &dwBytesWritten, NULL);
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


int main(int argc, char* argv[]) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
 
    LPSTR filename;
    if (argc != 2) {
        _print(hStdErr, "Please specify file to read as a single argument (e.g ./linux <filename>)\n");
        return -1;
    } else {
        filename = argv[1];
    }

    HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        _perror(hStdErr, "Error occurred while opening a file");
    }

    LPSTR buff = (LPSTR)malloc(BUFF_SIZE);
    if (buff == NULL) {
        _perror(hStdErr, "Error occurred while allocating memory for a buffer");
        CloseHandle(hFile);
    }

    DWORD dwTotalBytesRead = 0, dwBytesRead = 1;
    while (dwBytesRead > 0) {
        if (!ReadFile(hFile, buff, BUFF_SIZE, &dwBytesRead, NULL)) {
            _perror(hStdErr, "Error occurred while reading a file");
            free(buff);
            CloseHandle(hFile);
            return -3;
        }
        dwTotalBytesRead += dwBytesRead;
    }
    
    CHAR totalBytesStr[12];
    sprintf(totalBytesStr, "%zi", dwTotalBytesRead);

    LPSTR msgs[] = {"File read successfully!\n", 
                    "Total size of file is ", totalBytesStr, " bytes\n"};
    _print_many(hStdOut, msgs, 4);
    
    free(buff);
    CloseHandle(hFile);
    return 0; 
}