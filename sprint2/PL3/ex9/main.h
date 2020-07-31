#ifndef PL03_H_
#define PL03_H_

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Range of random numbers (0 - 1000)
#define NUMBER_RANGE 1000
// Amount of random numbers generated
#define NUMBER_AMOUNT 1000
// Size of array with biggest numbers
#define ARRAY_SIZE 10
// Name of file
#define FILE "/ex9"

typedef struct
{
    int arr[ARRAY_SIZE];
} shared_data_type;

#endif /* PL03_H_ */
