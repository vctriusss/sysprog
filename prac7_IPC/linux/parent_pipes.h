#include <unistd.h>
#include <wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mem.h"
#include "scan.h"


int parent_pipes(int N, int totalNums, FILE *file) {
    int nEach = totalNums / N;
    int nLast = totalNums - (N - 1) * nEach;

    pid_t *pids = malloc(N * sizeof(pid_t));
    if (pids == NULL) {
        perror("Failed to malloc for pids");
        return 1;
    }
    int *parent_read_pipes = malloc(N * sizeof(int));
    if (parent_read_pipes == NULL) {
        perror("Failed to malloc for pipes");
        return 1;
    }
    
    for (int i = 0; i < N; i++) {
        int nWrite = (i == N - 1) ? nLast : nEach;
        char nWriteStr[INT_LEN];
        sprintf(nWriteStr, "%d", nWrite);

        int fds[2];
        if (pipe(fds) < 0) {
            perror("Failed to create pipes");
            return 1;
        }
        int child_read_pipe = fds[0];
        int parent_write_pipe = fds[1];

        int fdss[2];
        if (pipe(fdss) < 0) {
            perror("Failed to create pipes");
            return 1;
        }
        int parent_read_pipe = fdss[0];
        int child_write_pipe = fdss[1];
        parent_read_pipes[i] = parent_read_pipe;

        pid_t pid = fork();
        switch (pid) {
        case 0:
            dup2(child_read_pipe, STDIN_FILENO);
            dup2(child_write_pipe, STDOUT_FILENO);
            close(child_read_pipe);
            close(child_write_pipe);
            close(parent_read_pipe);
            close(parent_write_pipe);
            
            char *args[2] = {nWriteStr, NULL};
            if (execve("./child_pipes", args, NULL) == -1) {
                perror("Failed to spawn child process");
                return 1;
            }
        case -1:
            perror("Error after spawning child process");
            return 1;
        default:
            pids[i] = pid;
            close(child_read_pipe);
            close(child_write_pipe);
        }

        // writing to pipes
        for (int j = 0; j < nWrite; j++) {
            char numStr[INT_LEN];
            scanInt(file, numStr);

            if (write(parent_write_pipe, numStr, strlen(numStr)) == -1) {
                perror("Failed to write to pipe from parent");
                return 1;
            }
            if (write(parent_write_pipe, " ", 1) == -1) {
                perror("Failed to write to pipe from parent");
                return 1;
            }
        }
        close(parent_write_pipe);
    }
    fclose(file);

    for (int i = 0; i < N; i++) {
        int code;
        waitpid(pids[i], &code, 0);
        if (WEXITSTATUS(code) != 0) {
            RaiseError("Child subprocess finished unsuccessfully");
            return 1;
        }
    }
    free(pids);

    double sum = 0;
    for (int i = 0; i < N; i++) {
        char doubleStr[FLOAT_LEN];

        if (scanDouble(parent_read_pipes[i], doubleStr) < 0) {
            RaiseError("Failed to scan double from parent");
            return 1;
        }
        sum += strtod(doubleStr, NULL);
        close(parent_read_pipes[i]);
    }

    free(parent_read_pipes);
    printf("The final sum of square roots is: %lf\n", sum);
    return 0;
}