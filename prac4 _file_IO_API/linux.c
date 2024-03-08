#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#define BUFF_SIZE 1024


void _print(int stream, const char *msg) {
    write(stream, msg, strlen(msg));
}


void _print_many(int stream, char *messages[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        _print(stream, messages[i]);
    }
}


void _perror(char *msg) {
    char *msgs[4] = {msg, ": ", strerror(errno), "\n"};
    _print_many(STDERR_FILENO, msgs, 4);
}


int main(int argc, char* argv[]) {
    char* filename;
    if (argc != 2) {
        _print(STDERR_FILENO, "Please specify file to read as a single argument (e.g ./linux <filename>)\n");
        return -1;
    } else {
        filename = argv[1];
    }

    int file = open(filename, O_RDONLY);
    if (file < 0) {
        _perror("Error occurred while opening a file");
        return -2;
    }

    char *buff = (char *)malloc(BUFF_SIZE);
    if (buff == NULL) {
        _perror("Error occurred while allocating memory for a buffer");
        close(file);
    }

    ssize_t totalBytesRead = 0, bytesRead = 0;
    while ((bytesRead = read(file, buff, BUFF_SIZE)) > 0) {
        totalBytesRead += bytesRead;
    }
    if (bytesRead < 0) {
        _perror("Error occurred while reading a file");
        free(buff);
        close(file);
        return -3;
    }
    
    char totalBytesStr[12];
    sprintf(totalBytesStr, "%zi", totalBytesRead);

    char* msgs[] = {"File read successfully!\n", 
                    "Total size of file is ", totalBytesStr, " bytes\n"};
    _print_many(STDOUT_FILENO, msgs, 4);

    free(buff);
    close(file);
    return 0; 
}