#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"

int WriteToFile(char *path, char buff[], size_t size);

int ReadFromFile(char *path, char buff[], size_t size);

int Scan(char dest[], size_t size, bool isCode);

int GetFilePath(char dest[], int argc, char *argv[]);

bool isValid(char c, bool isCodeChar);