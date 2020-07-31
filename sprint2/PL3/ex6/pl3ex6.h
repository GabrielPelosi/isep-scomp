#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <wait.h>
#include <time.h>

// Function to transfer data using pipes
void pp();

// Function to transfer data using shared memory
void shared();

// Size of array
#define ARRAY_SIZE 1000000

// Size of data being transfered in pipes
#define PIPE_DATA_SIZE sizeof(int)

// Name of memory area
#define MEMORY_AREA_NAME "/PL3EX6_memory_area"

// Shared memory area
typedef struct
{
    int numbers[ARRAY_SIZE];
    int sem;
} shared_memory_area;
