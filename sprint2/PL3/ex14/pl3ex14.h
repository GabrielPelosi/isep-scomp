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

// Number of integers to store in the shared memory area
#define ARRAY_SIZE 10

// Number of transfers to be done
#define NUM_TRANSFER 3

// Name of shared memory area
#define SHARED_MEMORY_AREA "/pl3ex14_shrd_mem_area"

// Structure of shared memory area
typedef struct
{
    int numbers[ARRAY_SIZE];
    int sem;
} shared_data;