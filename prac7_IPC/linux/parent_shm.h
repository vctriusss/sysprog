#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "mem.h"
#include "scan.h"


int parent_shm(int N, int totalNums, FILE* file) {
    int nEach = totalNums / N;
    int nLast = totalNums - (N - 1) * nEach;

    int nums_fd = shm_open("/nums", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (nums_fd == -1) {
        perror("Failed to open shared memory");
        return 1;
    }
    if (ftruncate(nums_fd, totalNums * sizeof(int)) == -1){
        perror("Failed to truncate shared memory");
        return 1;
    }
    int *nums_mem = (int*)mmap(NULL, totalNums * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, nums_fd, 0);
    if (nums_mem == MAP_FAILED) {
        perror("Failed to map memory");
        return 1;
    }

    int res_fd = shm_open("/res", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (res_fd == -1) {
        perror("Failed to open shared memory");
        return 1;
    }
    ftruncate(res_fd, 1 * sizeof(double));
    if (ftruncate(res_fd, totalNums * sizeof(int)) == -1){
        perror("Failed to truncate shared memory");
        return 1;
    }
    double *res_mem = (double*)mmap(NULL, 1 * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, res_fd, 0);
    if (res_mem == MAP_FAILED) {
        perror("Failed to map memory");
        return 1;
    }

    int mu_fd = shm_open("/mutex", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (mu_fd == -1) {
        perror("Failed to open shared memory");
        return 1;
    }
    ftruncate(mu_fd, 1 * sizeof(pthread_mutex_t));
    if (ftruncate(mu_fd, totalNums * sizeof(int)) == -1){
        perror("Failed to truncate shared memory");
        return 1;
    }
    pthread_mutex_t *mu_mem = (pthread_mutex_t*)mmap(NULL, 1 * sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mu_fd, 0);
    if (mu_mem == MAP_FAILED) {
        perror("Failed to map memory");
        return 1;
    }

    pthread_mutexattr_t mta;
    pthread_mutexattr_init(&mta);
    pthread_mutexattr_setpshared(&mta, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mu_mem, &mta);

    pid_t *pids = (pid_t*)malloc(N * sizeof(pid_t));
    if (pids == NULL) {
        perror("Failed to alloc memory for pids");
        return 1;
    }

    char numStr[INT_LEN];
    int num;
    int *ptr = nums_mem;

    for (int i = 0; i < N; i++) {
        int nWrite = (i == N - 1) ? nLast : nEach;

        for (int j = 0; j < nWrite; j++) {
            scanInt(file, numStr);
            num = atoi(numStr);
            memcpy(ptr++, &num, sizeof(int));
        }

        pid_t pid = fork();
        switch (pid) {
        case 0:
            char nWriteStr[INT_LEN];
            sprintf(nWriteStr, "%d", nWrite);

            char offsetStr[INT_LEN];
            sprintf(offsetStr, "%d", i * nEach);

            char totalStr[INT_LEN];
            sprintf(totalStr, "%d", totalNums);

            char *args[4] = {offsetStr, nWriteStr, totalStr, NULL};
            if (execve("./child_shm", args, NULL) == -1) {
                perror("Failed to spawn child process");
                return 1;
            }
            break;
        case -1:
            perror("Error after spawning child process");
            return 1;
        default:
            pids[i] = pid;
        }
    }

    for (int i = 0; i < N; i++) {
        int code;
        waitpid(pids[i], &code, 0);
        if (WEXITSTATUS(code) != 0) {
            RaiseError("Child subprocess finished unsuccessfully");
            return 1;
        }
    }
    if (shm_unlink("/nums") == -1) {
        perrror("Failed to unlink shared mem");
        return 1;
    }
    if (shm_unlink("/res") == -1) {
        perrror("Failed to unlink shared mem");
        return 1;
    }
    if (shm_unlink("/mutex") == -1) {
        perrror("Failed to unlink shared mem");
        return 1;
    };

    free(pids);
    pthread_mutex_destroy(mu_mem);

    printf("The final sum of square roots is: %lf\n", *res_mem);
    return 0;
}