#include <stdio.h>

#define ERR_INCORRECT_MODE -1
#define ERR_TOO_LONG -2
#define ERR_TOO_SHORT -3
#define ERR_DEPRECATED_CHAR -4
#define ERR_OPENING_FILE -5
#define ERR_BROKEN_FILE -6
#define ERR_INVALID_FLAG -7

void RaiseError(char *msg);