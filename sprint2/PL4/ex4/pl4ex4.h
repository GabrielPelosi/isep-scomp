#ifndef PL4_H
#define PL4_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

// Name of shared memory area
#define MEM_AREA "/pl4ex3_shared_mem_area"
// Number of processes to create
#define NUM_PROCESS 4
// String to be added to memory area
#define STR "I'm the Father - with PID "
// Minimum number of seconds to wait
#define MIN_SECONDS 1
// Maximum number of seconds to wait
#define MAX_SECONDS 5
// Number of seconds to wait to remove last string
#define STRING_RMV_SENCONDS 12
// Name of semaphore
#define SEMAPHORE "/pl4ex3_sem"
// Number of strings
#define NUM_STRINGS 50
// Number of characters in each string
#define NUM_CHARS 80

// Shared memory area with array of 50 strings of 80 characters
typedef struct{
    int index;
    char s[NUM_STRINGS][NUM_CHARS];
}shared_data_type;

#endif