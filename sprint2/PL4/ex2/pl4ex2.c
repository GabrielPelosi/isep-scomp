#include "pl4ex2.h"

int main(void)
{
    int i, j, num;
    char semName[100];
    FILE *inputFile, *outputFile;
    pid_t pid;
    sem_t *sem[NUMBER_PROCESS];

    // Create semaphores

    /* First semaphore is initialized at 1 */
    sprintf(semName, "%s%d", SEMAPHORE, 1);
    if ((sem[0] = sem_open(semName, O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }
    /* Other semaphores are initialized at 0 */
    for (i = 1; i < NUMBER_PROCESS; i++)
    {
        sprintf(semName, "%s%d", SEMAPHORE, i + 1);
        if ((sem[i] = sem_open(semName, O_CREAT, 0644, 0)) == SEM_FAILED)
        {
            perror("Error in sem_open()");
            exit(1);
        }
    }

    // Create clean output file
    outputFile = fopen(OUTPUT_FILE, "w");
    fclose(outputFile);

    // Creating 8 processes
    for (i = 0; i < NUMBER_PROCESS; i++)
    {
        pid = fork();
        if (pid == 0) /* Child process */
        {
            sem_wait(sem[i]);

            // Open input file for reading
            inputFile = fopen(INPUT_FILE, "r");
            if (inputFile != NULL)
            {
                // Open output file for writing at the end of the file
                outputFile = fopen(OUTPUT_FILE, "a");

                // Set beginning point of reading
                for (j = 0; j < i * NUMBER_NUMBERS; j++)
                {
                    fscanf(inputFile, "%d", &num);
                }
                // Read the numbers
                for (; j < (i + 1) * NUMBER_NUMBERS; j++)
                {
                    fscanf(inputFile, "%d", &num);
                    fprintf(outputFile, "%d\n", num);
                }

                // Close output file
                fclose(outputFile);

                // Close input file
                fclose(inputFile);
            }
            // Opening next semaphore
            if (i < NUMBER_PROCESS - 1)
            {
                sem_post(sem[i + 1]);
            }

            // Close semaphore
            sem_close(sem[i]);
            // Unlink from memory
            sprintf(semName, "%s%d", SEMAPHORE, i + 1);
            sem_unlink(semName);

            exit(0);
        }
        else /* Parent process */
        {
            wait(NULL);
        }
    }
    // Open output file for reading
    outputFile = fopen(OUTPUT_FILE, "r");
    if (outputFile == NULL)
    {
        printf("Could not open file");
    }
    else
    {
        while (fscanf(outputFile, "%d", &num) != EOF)
        {
            printf("%d\n", num);
        }
    }
    fclose(outputFile);

    return EXIT_SUCCESS;
}