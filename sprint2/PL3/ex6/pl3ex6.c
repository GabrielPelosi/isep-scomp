#include "pl3ex6.h"

int main(void)
{
    pp();
    shared();
    return 0;
}

void pp()
{
    int i = 0, aux, array[ARRAY_SIZE], fd[2];
    double time;
    pid_t pid;
    time_t start, finish;

    pipe(fd);

    // Mark start time
    start = clock();

    pid = fork();

    if (pid == 0) /* Child process */
    {
        // Close writer
        close(fd[1]);
        // Read numbers from pipe
        while (read(fd[0], &aux, PIPE_DATA_SIZE))
        {
            array[i] = aux;
            i++;
        }
        exit(0);
    }
    else /* Parent process */
    {
        // Close reader
        close(fd[0]);
        // Write numbers in pipe
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            aux = i;
            write(fd[1], &aux, PIPE_DATA_SIZE);
        }
        // Close writer
        close(fd[1]);
    }
    wait(NULL);
    // Mark finish time
    finish = clock();
    // Calculate number of seconds passed
    time = (((double)(finish - start)) / CLOCKS_PER_SEC);

    printf("Transferência por pipe efetuada em %.2f segundos\n", time);
}

void shared()
{
    int i, fd, array[ARRAY_SIZE], data_size = sizeof(shared_memory_area);
    double time;
    time_t start, finish;
    pid_t pid;
    shared_memory_area *mem_area;

    // Mark start time
    start = clock();

    fd = shm_open(MEMORY_AREA_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    mem_area = (shared_memory_area *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid = fork();

    // Setting switch to 0
    mem_area->sem = 0;

    if (pid == 0) /* Child process */
    {
        // Wait for switch to turn to 1
        while (mem_area->sem)
            ;
        // Writing numbers in memory area
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            mem_area->numbers[i] = i;
        }
        exit(0);
    }
    else /* Parent process */
    {
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            array[i] = mem_area->numbers[i];
        }
    }
    wait(NULL);

    // Unlink shm
    if (shm_unlink(MEMORY_AREA_NAME) < 0)
    {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }
    // Disconnect map
    if (munmap(mem_area, data_size) < 0)
    {
        printf("Error  munmap()!\n");
        exit(EXIT_FAILURE);
    }
    // Close fd
    if (close(fd) < 0)
    {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

    // Mark finish time
    finish = clock();
    // Calculate time
    time = ((double)(finish - start)) / CLOCKS_PER_SEC;

    printf("Transferência por shared memory area efetuada em %.2f segundos\n", time);
}
