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

// Size of string of name of student
#define STR_SIZE 50

// Number of classes of student
#define NR_DISC 10

// Name of shared memory area
#define MEM_AREA_NAME "/PL3EX12_mem_area"

// Shared memory area with student information and switch
typedef struct{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
    int sem;
}shared_mem_type;