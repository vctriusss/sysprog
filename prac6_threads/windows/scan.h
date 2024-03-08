#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>

#include "error.h"

#define ll long long
#define MAX_NUM 9007199254740992LL


int scanInt(FILE *file, char dest[16]) {
    int max_size = 16;
    char buff[max_size];

    for (int j = 0; j < max_size; j++) 
        buff[j] = 0;
    
    char tmp = fgetc(file);
    size_t i = 0;

    while (tmp != ' ' && !feof(file)) {
        if (i == max_size) {
            RaiseError("Number of digits is too high");
            return -123;
        }
        if (tmp < '0' || tmp > '9') {
            RaiseError("Number must contain only digits");
            return -123;
        }
        buff[i++] = tmp;
        tmp = fgetc(file);
    }

    if (i == 0) {
        RaiseError("There must be exacly 1 spaces between numbers and no trailing spaces");
        return -123;
    }

    char *endPtr;
    ll result = strtoll(buff, &endPtr, 10);

    if (((result == LLONG_MIN || result == LLONG_MAX) && errno == ERANGE) || result > MAX_NUM) {
        RaiseError("Number is out of range. It should be a positive integer from 1 to 9007199254740992");
        return -1;
    }

    strcpy(dest, buff);
    return 0;
}


int scanDouble(FILE *file, char dest[32]) {
    int max_size = 32;
    char buff[max_size];

    for (int j = 0; j < max_size; j++) 
        buff[j] = 0;
    

    char tmp = fgetc(file);
    size_t i = 0;

    while (tmp != ' ' && !feof(file)) {
        if (i == max_size) {
            RaiseError("Number of digits is too high");
            return -123;
        }
        if ((tmp < '0' || tmp > '9') && tmp != '.') {
            RaiseError("Number must contain only digits or `.`");
            return -123;
        }
        buff[i++] = tmp;
        tmp = fgetc(file);
    }
    if (i == 0) {
        RaiseError("There must be exacly 1 spaces between numbers and no trailing spaces");
        return -123;
    }

    char *endPtr;
    double result = strtod(buff, &endPtr);

    if ((result == HUGE_VAL || result == 0) && errno == ERANGE) {
        perror("Error converting to double");
        return -1;
    }

    strcpy(dest, buff);
    return 0;
}

