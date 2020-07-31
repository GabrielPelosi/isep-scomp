#include "main.h"

int main(void)
{
    int i, j, max, div, fd, data_size = sizeof(shared_data_type);
    int numbers[NUMBER_AMOUNT];
    shared_data_type *shared_data;

    fd = shm_open(FILE, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    ftruncate(fd, data_size);

    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Number used for each process to find maximum
    div = NUMBER_AMOUNT / ARRAY_SIZE;

    // Initialize random generator using current time
    srand(time(0));

    // Create array with random numbers from 0 to 1000
    for (i = 0; i < NUMBER_AMOUNT; i++)
        numbers[i] = rand() % (NUMBER_RANGE + 1);

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        if (fork() == 0) /* Child process */
        {
            shared_data->arr[i] = 0;
            for (j = div * i; j < div * (i + 1); j++)
            {
                if (numbers[j] > shared_data->arr[i])
                    shared_data->arr[i] = numbers[j];
            }
            // Print maximum
            printf("Maximum found: %d (%d)\n", shared_data->arr[i], i);

            // Disconnect map
            if (munmap(shared_data, data_size) < 0)
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

            // Exit child process
            exit(EXIT_SUCCESS);
        }
        else /* Parent process */
        {
            wait(NULL);
        }
    }

    // Parent process
    max = 0;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        if (shared_data->arr[i] > max)
            max = shared_data->arr[i];
    }

    // Print maximum
    printf("Max: %d\n", max);

    // Unlink shm
    if (shm_unlink(FILE) < 0)
    {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    // Disconnect map
    if (munmap(shared_data, data_size) < 0)
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

    wait(0);
    return EXIT_SUCCESS;
}