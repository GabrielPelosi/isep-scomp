#include "pl4ex4.h"

int main(void)
{
    int i = 0, op, fd, data_size = sizeof(shared_data_type);
    sem_t *sem;
    shared_data_type *mem_area;

    // Use current time as seed for random generator
    srand(time(0));

    // Create memory area
    fd = shm_open(MEM_AREA, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    mem_area = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Create semaphore
    if ((sem = sem_open(SEMAPHORE, O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    // Create 4 processes
    for (i = 0; i < NUM_PROCESS; i++)
    {
        if (fork() == 0) /* Child process */
        {
            // Block semaphore
            sem_wait(sem);

            // Writing in memory area
            sprintf(mem_area->s[mem_area->index], "%s%d", STR, getpid());

            // Increasing the index
            mem_area->index++;
            if (mem_area->index >= NUM_STRINGS)
            {
                mem_area->index = 0;
            }

            // Raise semaphore
            sem_post(sem);

            // Waiting random number between 1 and 5 seconds
            sleep((rand() % (MAX_SECONDS - MIN_SECONDS + 1)) + MIN_SECONDS);

            // Close semaphore
            sem_close(sem);

            exit(0);
        }
        else
        {
            wait(NULL);
        }
    }

    /* Parent process */

    // Print strings in memory area
    for (i = 0; i < mem_area->index; i++)
    {
        printf("%s\n", mem_area->s[i]);
    }

    // Eliminate string option
    do
    {
        printf("\n1 - Remove the last string from shared memory\n0 - Cancel\n");
        scanf("%d", &op);
        switch (op)
        {
        case 1: /* remove last string */
            printf("Removing last string...\n");

            // Sleep 12 seconds
            sleep(STRING_RMV_SENCONDS);
            // Remove string from last position
            strcpy(mem_area->s[mem_area->index - 1], "" /* empty string */);
            // Decrement index position
            mem_area->index--;
            // Print strings in memory area
            for (i = 0; i < mem_area->index; i++)
            {
                printf("%s\n", mem_area->s[i]);
            }

            break;

        case 0: /* continue */
            printf("Cancelled\n");
            break;

        default:
            printf("Sintax error. Insert a valid option\n");
            sleep(2);
        }

    } while (op != 0);

    // Unlink shm
    if (shm_unlink(MEM_AREA) < 0)
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

    // Unlink semaphore
    sem_unlink(SEMAPHORE);

    return 0;
}