#include "pl3ex15.h"

int main(void)
{
    int i, fd[2];
    pid_t pid;

    pipe(fd);
    pid = fork();

    if (pid == 0) /* Producer */
    {
        // Close reader
        close(fd[0]);

        // Writing 30 numbers through pipe
        for (i = 1; i <= NUM_TRANSFER; i++)
        {
            write(fd[1], &i, DATA_SIZE);
        }
    }
    else /* Consumer */
    {
        // Close writer
        close(fd[1]);

        // Reading 30 numbers through pipe
        while (read(fd[0], &i, DATA_SIZE) > 0)
        {
            printf("%d\n", i);
        }
        exit(0);
    }
    wait(NULL);

    return 0;
}
