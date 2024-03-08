#pragma once

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>

#include "error.h"

#define ll long long
#define MAX_NUM 9007199254740992LL


int scanIntPipe(int file, char dest[16]) {
    int max_size = 16;
    char buff[max_size];

    for (int j = 0; j < max_size; j++) 
        buff[j] = 0;
    
    char tmpbuff[1];
    if (read(file, tmpbuff, 1) == -1) {
        perror("Failed to read from file");
        return -123;
    }
    char tmp = tmpbuff[0];

    size_t i = 0;
    while (tmp != ' ') {
        buff[i++] = tmp;
        if (read(file, tmpbuff, 1) == -1) {
            perror("Failed to read from file");
            return -123;
        }
        tmp = tmpbuff[0];
    }

    strcpy(dest, buff);
    return 0;
}

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

int scanDouble(int file, char dest[32]) {
    int max_size = 32;
    char buff[max_size];

    for (int j = 0; j < max_size; j++) 
        buff[j] = 0;
    
    char tmpbuff[1];
    ssize_t res = read(file, tmpbuff, 1);
    if (res == -1) {
        perror("Failed to read from file");
        return -123;
    }
    char tmp = tmpbuff[0];

    size_t i = 0;
    while (tmp != ' ') {
        if (i == max_size) {
            RaiseError("Number of digits is too high");
            return -123;
        }
        if ((tmp < '0' || tmp > '9') && tmp != '.') {
            RaiseError("Number must contain only digits or `.`");
            return -123;
        }
        buff[i++] = tmp;
        res = read(file, tmpbuff, 1);
        if (res == -1) {
            perror("Failed to read from file");
            return -123;
        }
        tmp = tmpbuff[0];
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

