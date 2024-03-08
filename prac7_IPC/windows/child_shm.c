#include "scan.h"
#include "mem.h"


int main(int argc, char **argv) {
    int offset = atoi(argv[0]);
    int nWrite = atoi(argv[1]);
    int totalNums = atoi(argv[2]);

    HANDLE StdErr = GetStdHandle(STD_ERROR_HANDLE);

    HANDLE nums_fd = OpenFileMappingA(FILE_MAP_READ, FALSE, "nums");
    if (nums_fd == NULL) {
        _perror(StdErr, "Failed to open shared memory");
        ExitProcess(1);
    }
    int *nums_mem = (int*)MapViewOfFile(nums_fd, FILE_MAP_READ, 0, 0, totalNums * sizeof(int));
    if (nums_mem == NULL) {
        _perror(StdErr, "Failed to map memory");
        ExitProcess(1);
    }

    HANDLE res_fd = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "res");
    if (res_fd == NULL) {
        _perror(StdErr, "Failed to open shared memory");
        ExitProcess(1);
    }
    double *res_mem = (double*)MapViewOfFile(res_fd, FILE_MAP_ALL_ACCESS, 0, 0, 1 * sizeof(double));
    if (res_mem == NULL) {
        _perror(StdErr, "Failed to map memory");
        ExitProcess(1);
    }

    HANDLE mu = OpenMutex(MUTEX_ALL_ACCESS, TRUE, "mu");
    if (mu == NULL) {
        _perror(StdErr, "Failed to open mutex");
        ExitProcess(1);   
    }

    for (int i = offset; i < offset + nWrite; i++) {
        if (WaitForSingleObject(mu, INFINITE) == WAIT_FAILED) {
            _perror(StdErr, "Failed to lock mutex");
            ExitProcess(1);
        }
        *res_mem += sqrt((double)nums_mem[i]);

        if (!ReleaseMutex(mu)) {
            _perror(StdErr, "Failed to unlock mutex");
            ExitProcess(1);
        }
    }

    if (!UnmapViewOfFile(nums_mem)) {
        _perror(StdErr, "Failed to unmap file view");
        return 1;
    }
    if (!UnmapViewOfFile(res_mem)) {
        _perror(StdErr, "Failed to unmap file view");
        return 1;
    }
    CloseHandle(nums_fd);
    CloseHandle(res_fd);

    ExitProcess(0);
}