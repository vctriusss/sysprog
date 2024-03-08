#include "io.h"

int WriteToFile(char *path, char buff[], size_t size) {
    FILE *file = fopen(path, "w");
    if (file == NULL)
        return ERR_OPENING_FILE;

    for (size_t i = 0; i < size; i++) {
        fputc(buff[i], file);
    }
    fclose(file);
    return 0;
}

int ReadFromFile(char *path, char buff[], size_t size) {
    FILE *file = fopen(path, "r");
    if (file == NULL)
        return ERR_OPENING_FILE;

    size_t i = 0;
    for (; i < size && !feof(file); i++) {
        buff[i] = fgetc(file);
    }

    if (i != size)
        return ERR_BROKEN_FILE;

    fclose(file);
    return 0;
}

int Scan(char dest[], size_t size, bool isCode) {
    char tmp = getchar();

    size_t i = 0;

    while (tmp != '\n') {
        if (i == size) // input is too long
            return ERR_TOO_LONG;

        if (!isValid(tmp, isCode))
            return ERR_DEPRECATED_CHAR;

        dest[i] = isCode ? toupper(tmp) : tmp; // capitalize for code
        i++;
        tmp = getchar();
    }

    if (i == 0) // no input
        return ERR_TOO_SHORT;
    if (isCode && i != size) // code is too short
        return ERR_TOO_SHORT;

    return 0;
}

bool isValid(char c, bool isCodeChar) {
    if (isCodeChar)
        return isalpha(c);

    return isalnum(c) || c == ' ' || c == '\'' || c == '"';
}

int GetFilePath(char dest[], int argc, char *argv[]){
    if (argc != 3 || strcmp(argv[1], "-f") != 0) {
        return ERR_INVALID_FLAG;
    }

    strcpy(dest, argv[2]);
    return 0;
}