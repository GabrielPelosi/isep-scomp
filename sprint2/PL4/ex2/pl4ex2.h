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

// Number of processes
#define NUMBER_PROCESS 8
// Number of numbers to be used in the files
#define NUMBER_NUMBERS 200
// Name of file to read
#define INPUT_FILE "Numbers.txt"
// Name of file to write
#define OUTPUT_FILE "Output.txt"
// Name of semaphores
#define SEMAPHORE "/pl4ex2_sem"

#endif
