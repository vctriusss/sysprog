#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define RND_MAX 1000


void printMatrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {

        printf("%d\t", matrix[i]);

        if (i % cols == cols - 1) 
            printf("\n");
        
    }
}

int scanInt() {
    int max_size = 256;
    char buff[max_size];

    for (int j = 0; j < max_size; j++) {
        buff[j] = 0;
    }

    char tmp = getchar();

    size_t i = 0;

    while (tmp != '\n') {
        if (i == max_size) {
            fprintf(stderr, "[ERROR] Number of digits is too high");
            return -123;
        }

        if ((tmp < '0' || tmp > '9') && tmp != '-') {
            fprintf(stderr, "[ERROR] Number must contain only digits");
            return -123;
        }
        buff[i] = tmp;
        i++;
        tmp = getchar();
    }

    if (i == 0) {
        fprintf(stderr, "[ERROR] Number is empty");
        return -123;
    }

    int res = atoi(buff);

    if (i > 10 || (i == 10 && res < 1e9)) {
        fprintf(stderr, "[WARN] Too huge number, there will be overflow\n");
    }

    return res;
}

extern void crossOut(int* matrix, int* newMatrix, int row, int col, int excl_row, int excl_col);

int main(int argc, char *argv[]) {
    int n, m, excl_row, excl_col;

    printf("Enter number of rows: ");
    n = scanInt();
    if (n == -123) {
        return -1;
    }
    if (n <= 0) {
        fprintf(stderr, "[ERROR] Size must be positive");
        return -1;
    }

    printf("Enter number of columns: ");
    m = scanInt();
    if (m == -123) {
        return -1;
    }
    if (m <= 0) {
        fprintf(stderr, "[ERROR] Size must be positive");
        return -1;
    }
    time_t seconds = time(NULL);
    srand(seconds);

    if (argc == 2 && strcmp(argv[1], "-f") != 0) {
        excl_row = rand() % n;
        excl_col = rand() % m;
    } else {

        printf("Enter row number to exclude (from %d to %d inclusively): ", 0, n - 1);
        excl_row = scanInt();
        // printf("%d", excl_row);
        if (excl_row == -123) {
            return -1;
        }
        if (excl_row < 0 || excl_row > n - 1) {
            fprintf(stderr, "[ERROR] Row number to exlude must be in range from %d to %d inclusively", 0, n - 1);
            return -2;
        }

        printf("Enter column number to exclude (from %d to %d inclusively): ", 0, m - 1);
        excl_col = scanInt();
        if (excl_col == -123) {
            return -1;
        }
        if (excl_col < 0 || excl_col > m - 1) {
            fprintf(stderr, "[ERROR] Column number to exlude must be in range from %d to %d inclusively", 0, m - 1);
            return -2;
        }
    }



    // malloc and initialize matrix
    int *matrix = (int*)malloc(n * m * sizeof(int));
    if (matrix == NULL) {
        perror("Error allocating memory");
        return -1;
    }
    for (int i = 0; i < n * m; i++) {
        // matrix[i] = i + 1;
        matrix[i] = rand() % RND_MAX;
    }

    printf("Initial matrix is:\n");
    printMatrix(matrix, n, m);
    
    // malloc for new matrix
    int *newMatrix = (int*)malloc((n - 1) * (m - 1) * sizeof(int));
    if (newMatrix == NULL) {
        perror("Error allocating memory");
        return -1;
    }

    crossOut(matrix, newMatrix, m, n * m, excl_row, excl_col);

    printf("\nNew matrix is:\n");
    printMatrix(newMatrix, n - 1, m - 1);

    //free heap mem from matrices
    free(matrix);
    free(newMatrix);

    return 0;
}