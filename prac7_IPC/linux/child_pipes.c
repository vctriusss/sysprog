#include "scan.h"
#include "mem.h"


int main(int argc, char **argv) {
    int N = atoi(argv[0]);

    double sum = 0;

    for (int i = 0; i < N; i++) {
        char numStr[INT_LEN];
        scanIntPipe(STDIN_FILENO, numStr);

        char *endPtr;
        double num = strtod(numStr, &endPtr);
        if ((num == HUGE_VAL || num == 0) && errno == ERANGE) {
            perror("Error converting to double");
            exit(1);
        }
        sum += sqrt(num);
    }

    char sumStr[FLOAT_LEN+1];
    sprintf(sumStr, "%lf ", sum);
    if (write(STDOUT_FILENO, sumStr, strlen(sumStr)) == -1) {
        perror("Failed to write to pipe from child");
        exit(1);
    }

    exit(0);
}