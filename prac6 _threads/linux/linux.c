#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "mem.h"
#include "scan.h"


struct ChildArg {
    int iStart;
    int iEnd;
};


static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
double totalSum = 0;
char **nums;


void *process(void *arguments) {
    struct ChildArg *args = (struct ChildArg *) arguments;
    double localSum = 0;

    for (int i = args->iStart; i < args->iEnd; i++) {
        char *endPtr;
        double num = strtod(nums[i], &endPtr);
        if ((num == HUGE_VAL || num == 0) && errno == ERANGE) {
            perror("Error converting to double");
            exit(1);
        }
        localSum += sqrt(num);
    }

    if (pthread_mutex_lock(&mu)) {
        perror("Failed to lock mutex");
        exit(1);
    }
    
    totalSum += localSum;

    if (pthread_mutex_unlock(&mu)) {
        perror("Failed to unlock mutex");
        exit(1);
    }
    sleep(20);
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
    
    nums = (char **)malloc(MAX_NUMS * sizeof(char *));
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

    int iStart = 0;

    pthread_t *threads = (pthread_t *)malloc(N * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Failed to malloc for threads");
        freeNums(nums);
        exit(1);
    }
    struct ChildArg *childArgs = (struct ChildArg *)malloc(N * sizeof(struct ChildArg));
    if (childArgs == NULL) {
        perror("Failed to malloc for child args");
        freeNums(nums);
        free(threads);
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        int nWrite = (i == N - 1) ? nLast : nEach;

        childArgs[i].iStart = iStart;
        childArgs[i].iEnd = iStart + nWrite;

        iStart = childArgs[i].iEnd;

        if (pthread_create(&threads[i], NULL, process, &childArgs[i])) {
            perror("Failed to create new thread");
            freeNums(nums);
            free(threads);
            free(childArgs);
            exit(1);
        }
    }

    for (int i = 0; i < N; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("Failed to wait(join) running thread");
            freeNums(nums);
            free(threads);
            free(childArgs);
            exit(1);
        }
    }
    freeNums(nums);
    free(threads);
    free(childArgs);

    printf("The final sum of square roots is: %lf\n", totalSum);
    exit(0);
}