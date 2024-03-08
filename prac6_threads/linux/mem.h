#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMS 100000
#define MAX_FILENAME 32
#define INT_LEN 16
#define FLOAT_LEN 32


void closeAll(FILE **files, int n) {
    for (int i = 0; i < n; i++) {
        fclose(files[i]);
        free(files[i]);
    }
    free(files);
}

void freeNums(char **nums) {
    for (int i = 0; i < MAX_NUMS; i++) {
        free(nums[i]);
    }
    free(nums);
}