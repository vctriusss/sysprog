#include "scan.h"
#include "mem.h"


INT main(INT argc, LPSTR *argv) {
    HANDLE StdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE StdErr = GetStdHandle(STD_ERROR_HANDLE);

    INT N = atoi(argv[0]);

    double sum = 0;

    for (INT i = 0; i < N; i++) {
        CHAR numStr[INT_LEN];
        scanIntPipe(StdIn, numStr);

        CHAR *endPtr;
        double num = strtod(numStr, &endPtr);
        if ((num == HUGE_VAL || num == 0) && errno == ERANGE) {
            _perror(StdErr, "Error converting to double");
            ExitProcess(1);
        }
        sum += sqrt(num);
    }

    CHAR sumStr[FLOAT_LEN+1];
    sprintf(sumStr, "%lf ", sum);
    if (!WriteFile(StdOut, sumStr, strlen(sumStr), NULL, NULL)) {
        _perror(StdErr, "Failed to write to pipe from child");
        ExitProcess(1);
    }

    ExitProcess(0);
}