#include "pl3ex12.h"

int main(void)
{
    int i, fd, data_size = sizeof(shared_mem_type);
    int min = 200, max = 0, sum = 0;
    double avg;
    char c;
    pid_t pid;
    shared_mem_type *mem_area;

    fd = shm_open(MEM_AREA_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    mem_area = (shared_mem_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Turn memory area switch to 1
    mem_area->sem = 1;

    pid = fork();

    if (pid > 0) /* Parent process */
    {
        // Fill memory area with student information
        printf("Student number: ");
        scanf("%d", &mem_area->numero);
        while ((c = getchar()) != '\n' && c != EOF) /* Clear the return */
            ;
        printf("Student name: ");
        scanf("%[^\n]", mem_area->nome);
        printf("Student class grades: (Insert %d)\n", NR_DISC);
        for (i = 0; i < NR_DISC; i++)
        {
            printf("%d. ", i + 1);
            scanf("%d", &mem_area->disciplinas[i]);
        }

        // Turn switch to 0 to change process
        mem_area->sem = 0;
        wait(NULL);
    }
    else /* Child process */
    {
        while (mem_area->sem)
            ;
        for (i = 0; i < NR_DISC; i++)
        {
            // Lowest grade
            if (mem_area->disciplinas[i] < min)
                min = mem_area->disciplinas[i];
            // Highest grade
            if (mem_area->disciplinas[i] > max)
                max = mem_area->disciplinas[i];
            // Sum of all grades
            sum += mem_area->disciplinas[i];
        }
        // Average of all grades
        avg = ((double)sum) / NR_DISC;
        // Print student information
        printf("\nStudent number: %d\n", mem_area->numero);
        printf("Student name: %s\n", mem_area->nome);
        printf("Lowest grade: %d\n", min);
        printf("Highest grade: %d\n", max);
        printf("Average of all grades: %.2f\n", avg);

        exit(0);
    }
    // Parent process

    // Unlink shm
    if (shm_unlink(MEM_AREA_NAME) < 0)
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

    return 0;
}
