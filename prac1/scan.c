#include "scan.h"

#include <stdio.h>

int ScanInt(int* dest, int buffsize) {
    char tmp[buffsize];
    char extra[2 * buffsize];

    fgets(tmp, buffsize, stdin);
    int count = sscanf(tmp, "%d%s", dest, extra);

    return (count == 1) ? 1 : 0;
}

int ScanString(char* dest[], int buffsize) {
    char tmp[buffsize];

    fgets(tmp, buffsize, stdin);

    *dest = tmp;

    return 0;
}