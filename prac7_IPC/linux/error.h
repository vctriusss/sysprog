#pragma once

#include <stdio.h>

void RaiseError(char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
}

void RaiseWarn(char *msg) {
    fprintf(stdout, "[WARN] %s\n", msg);
}