#include "pl4ex1.h"

int main(void)
{
    int i = 0, j, num;
    FILE *inputFile, *outputFile;
    pid_t pid;
    sem_t *sem;

    // Create semaphore to close and clean
    if ((sem = sem_open(SEMAPHORE, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    // Create clean output file
    outputFile = fopen(OUTPUT_FILE, "w");
    fclose(outputFile);

    // Creating 8 processes
    do
    {
        pid = fork();
        i++;
    } while (pid > 0 && i < 8);

    if (pid == 0) /* Child process */
    {
        sem_wait(sem);

        // Open input file for reading
        inputFile = fopen(INPUT_FILE, "r");
        if (inputFile != NULL)
        {
            // Open output file for writing at the end of the file
            outputFile = fopen(OUTPUT_FILE, "a");

            for (j = 0; j < NUMBER_NUMBERS; j++)
            {
                fscanf(inputFile, "%d", &num);
                fprintf(outputFile, "%d\n", num);
            }

            // Close output file
            fclose(outputFile);

            // Close input file
            fclose(inputFile);
        }
        sem_post(sem);
        exit(0);
    }
    else /* Parent process */
    {
        wait(NULL);
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

    sem_close(sem);
    sem_unlink(SEMAPHORE);

    return EXIT_SUCCESS;
}