#include <unistd.h>
#include <wait.h>

#include "mem.h"
#include "scan.h"


pid_t startSubprocess(int i, char *n) {
    char fname[MAX_FILENAME];
    sprintf(fname, "../files/file%d.txt", i + 1);

    pid_t pid = fork();
    switch (pid) {
    case 0: {
        char *args[3] = {fname, n, NULL};
        execve("./child", args, NULL);
    }
    case -1: {
        perror("Error after spawning child process");
        exit(1);
        break;
    }
    default: {
        return pid;
    }
    }
}


int main(int argc, char **argv) {
    char* filename;
    int N;
    if (argc != 3) {
        RaiseError("Please specify file to read and number of processes accordingly (e.g ./linux <filename> <N>)");
        exit(1);
    } else {
        filename = argv[1];
        N = atoi(argv[2]);
        if (N <= 0) {
            RaiseError("Number of process must be a positive integer");
            exit(1);
        }
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    
    char **nums = (char **)malloc(MAX_NUMS * sizeof(char *));
    for (int i = 0; i < MAX_NUMS; i++) {
        nums[i] = (char *)malloc(16 * sizeof(char));
    }

    int totalNums = 0;
    while(!feof(file)) {
        char numStr[INT_LEN];
        if (scanInt(file, numStr) < 0) {
            fclose(file);
            freeNums(nums);
            exit(1);
        }
        strcpy(nums[totalNums++], numStr);

        if (totalNums == MAX_NUMS) {
            RaiseError("Too many numbers in file");
            fclose(file);
            freeNums(nums);
            exit(1);
        }
    }
    fclose(file);

    if (totalNums < 2) {
        RaiseError("There must be 2 or more numbers in a file");
        freeNums(nums);
        exit(1);
    }
    if (N > totalNums / 2) {
        RaiseWarn("There are too many child processes, decreasing...");
        N = totalNums / 2;
    }

    int nEach = totalNums / N;
    int nLast = totalNums - (N - 1) * nEach;

    for (int i = 0; i < N; i++) {
        int nWrite = (i == N - 1) ? nLast : nEach;
        char fname[MAX_FILENAME];
        sprintf(fname, "../files/file%d.txt", i + 1);

        FILE *childFile = fopen(fname, "w");
        if (childFile == NULL) {
            perror("Error opening file");
            freeNums(nums);
            fclose(childFile);
            exit(1);
        }

        for (int j = 0; j < nWrite; j++) {
            fwrite(nums[i * nEach + j], 1, strlen(nums[i * nEach + j]), childFile);
            if (j != nWrite - 1) {
                fwrite(" ", 1, 1, childFile);
            }
        }
        fclose(childFile);
    }

    freeNums(nums);
    pid_t *pids = (pid_t *)malloc(N * sizeof(pid_t));

    char nEachStr[INT_LEN], nLastStr[INT_LEN];
    sprintf(nEachStr, "%d", nEach);
    sprintf(nLastStr, "%d", nLast);

    for (int i = 0; i < N; i++) {
        pids[i] = startSubprocess(i, (i == N - 1) ? nLastStr: nEachStr);
    }
    
    // getchar(); 

    for (int i = 0; i < N; i++) {
        int code;
        waitpid(pids[i], &code, 0);
        if (WEXITSTATUS(code) != 0) {
            RaiseError("Child subprocess finished unsuccessfully");
            free(pids);
            exit(1);
        }
    }

    double sum = 0;
    for (int i = 0; i < N; i++) {
        char fname[MAX_FILENAME];
        sprintf(fname, "../files/file%d.txt", i + 1);

        FILE *childFile = fopen(fname, "r");
        if (childFile == NULL) {
            perror("Error opening file: ");
            fclose(childFile);
            free(pids);
            exit(1);
        }
        while (fgetc(childFile) != '\n' && !feof(childFile)) {}
        
        if (feof(childFile)) {
            RaiseError("No new data or newline character found in file");
            free(pids);
            fclose(childFile);
        }

        double num = 0;
        while(!feof(childFile)) {
            char numStr[FLOAT_LEN];
            if (scanDouble(childFile, numStr) < 0) {
                fclose(childFile);
                free(pids);
                exit(1);
            }
            char *endPtr;
            num = strtod(numStr, &endPtr);
            if ((num == HUGE_VAL || num == 0) && errno == ERANGE) {
                perror("Error converting to double:");
                fclose(childFile);
                free(pids);
                exit(1);
            }
        }
        sum += num;
        fclose(childFile);
    }

    printf("The final sum of square roots is: %lf\n", sum);
    exit(0);
}