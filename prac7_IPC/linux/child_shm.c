#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#include "scan.h"
#include "mem.h"


int main(int argc, char **argv) {
    int offset = atoi(argv[0]);
    int nWrite = atoi(argv[1]);
    int totalNums = atoi(argv[2]);

    int nums_fd = shm_open("/nums", O_RDONLY, S_IRUSR | S_IWUSR);
    if (nums_fd == -1) {
        perror("Failed to open shared memory");
        exit(1);
    }
    int *nums_mem = (int*)mmap(NULL, totalNums, PROT_READ, MAP_SHARED, nums_fd, 0);
    if (nums_mem == MAP_FAILED) {
        perror("Failed to map memory");
        exit(1);
    }

    int res_fd = shm_open("/res", O_RDWR, S_IRUSR | S_IWUSR);
    if (res_fd == -1) {
        perror("Failed to open shared memory");
        exit(1);
    }
    double *res_mem = (double*)mmap(NULL, 1 * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, res_fd, 0);
    if (res_mem == MAP_FAILED) {
        perror("Failed to map memory");
        exit(1);
    }

    int mu_fd = shm_open("/mutex", O_RDWR, S_IRUSR | S_IWUSR);
    if (mu_fd == -1) {
        perror("Failed to open shared memory");
        exit(1);
    }
    pthread_mutex_t *mu_mem = (pthread_mutex_t*)mmap(NULL, 1 * sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mu_fd, 0);
    if (mu_mem == MAP_FAILED) {
        perror("Failed to map memory");
        exit(1);
    }

    for (int i = offset; i < offset + nWrite; i++) {
        if (pthread_mutex_lock(mu_mem)) {
            perror("Failed to lock mutex");
            exit(1);
        }
        *res_mem += sqrt((double)nums_mem[i]);

        if (pthread_mutex_unlock(mu_mem)) {
            perror("Failed to unlock mutex");
            exit(1);
        }
    }

    munmap(nums_mem, 0);
    munmap(res_mem, 0);
    munmap(mu_mem, 0);

    exit(0);
}