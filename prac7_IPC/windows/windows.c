#include "parent_pipes.h"
#include "parent_shm.h"

typedef enum {
    ModePipes = 0x00,
    ModeSHM
} Mode;

LPSTR filename;
unsigned mode;
INT N;

INT main(int argc, char **argv) {
    HANDLE StdErr = GetStdHandle(STD_ERROR_HANDLE);

    if (argc != 4) {
        RaiseError("Please specify file to read, number of processes and IPC mode accordingly (e.g ./linux <filename> <N> <mode>)");
        exit(1);
    } else {
        filename = argv[1];
        N = atoi(argv[2]);
        if (N <= 0) {
            RaiseError("Number of process must be a positive integer");
            ExitProcess(1);
        }
        if (strcmp("pipes", argv[3]) != 0 && strcmp("shm", argv[3]) != 0) {
            RaiseError("IPC mode must be one of `pipes` or `shm`");
            ExitProcess(1);
        }
        mode = strcmp("pipes", argv[3]) ? ModeSHM : ModePipes;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        _perror(StdErr, "Error opening file");
        ExitProcess(1);
    }

    INT totalNums = 0;
    INT res = 0;
    while(!feof(file)) {
        CHAR numStr[INT_LEN];
        res = scanInt(file, numStr);
        if (res < 0) {
            fclose(file);
            ExitProcess(1);
        }
        totalNums++;
    }
    fseek(file, 0, 0);

    if (totalNums < 2) {
        RaiseError("There must be 2 or more numbers in a file");
        ExitProcess(1);
    }
    if (N > totalNums / 2) {
        RaiseWarn("There are too many child processes, decreasing...");
        N = totalNums / 2;
    }

    switch (mode) {
    case ModePipes:
        res = parent_pipes(N, totalNums, file);
        break;
    case ModeSHM:
        res = parent_shm(N, totalNums, file);
        break;
    }

    if (res) {
        RaiseError("Program finished unsuccessfully");
        ExitProcess(1);
    }

    ExitProcess(0);
}