#include "error.h"

void RaiseError(char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
}