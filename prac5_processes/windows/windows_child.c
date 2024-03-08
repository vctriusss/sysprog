#include "scan.h"
#include "mem.h"


int main(int argc, char **argv) {
    HANDLE StdErr = GetStdHandle(STD_ERROR_HANDLE);
    char* filename;
    int N;
    if (argc != 2) {
        RaiseError("Please specify file to read and number of numbers accordingly (e.g ./child <filename> <N>)");
        ExitProcess(1);
    } else {
        filename = argv[0];
        N = atoi(argv[1]);
        if (N <= 0) {
            RaiseError("Number of numbers must be a positive integer");
            ExitProcess(1);
        }
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        _perror(StdErr, "Error opening file");
        ExitProcess(1);
    }
    double sum = 0;
    int i = 0;

    while(!feof(file)) {
        char numStr[INT_LEN];
        if (scanInt(file, numStr) < 0) {
            fclose(file);
            ExitProcess(1);
        }
        char *endPtr;
        double num = strtod(numStr, &endPtr);
        if ((num == HUGE_VAL || num == 0) && errno == ERANGE) {
            perror("Error converting to double");
            ExitProcess(1);
        }
        sum += sqrt(num);
    }
    fclose(file);

    file = fopen(filename, "a");

    char sumStr[FLOAT_LEN + 1];
    snprintf(sumStr, sizeof(sumStr), "\n%lf", sum);
    fwrite(sumStr, 1, strlen(sumStr), file);
    
    fclose(file);
    // Sleep(5000);
    ExitProcess(0);
}