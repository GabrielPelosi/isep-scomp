#include "pl3ex14.h"

int main(void)
{
    int i, j, fd, data_size = sizeof(shared_data);
    pid_t pid;
    shared_data *mem_area;

    fd = shm_open(SHARED_MEMORY_AREA, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    mem_area = (shared_data *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    mem_area->sem = 1;

    pid = fork();

    if (pid == 0) /* Producer */
    {
        for (i = 0; i < NUM_TRANSFER; i++)
        {
            // Waiting for sem to turn to 1
            while (!mem_area->sem)
                ;
            // Writing numbers in memory area
            for (j = 0; j < ARRAY_SIZE; j++)
            {
                mem_area->numbers[j] = (i * 10) + j + 1;
            }
            // Turn switch to allow consumer to read
            mem_area->sem = 0;
        }
    }
    else /* Consumer */
    {
        for (i = 0; i < NUM_TRANSFER; i++)
        {
            // Waiting for sem to turn to 0
            while (mem_area->sem)
                ;
            // Reading numbers from memory area
            for (j = 0; j < ARRAY_SIZE; j++)
            {
                printf("%d\n", mem_area->numbers[j]);
            }
            // Turn switch to allow producer to write
            mem_area->sem = 1;
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
    }

    wait(NULL);

    if (pid > 0) /* Parent process */
    {
        // Unlink shm
        if (shm_unlink(SHARED_MEMORY_AREA) < 0)
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
    }

    return 0;
}