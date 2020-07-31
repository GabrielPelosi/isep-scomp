/*
 * pl03.h
 *
 *  Created on: 24/03/2020
 *      Author: isep
 */

#ifndef PL03_H_
#define PL03_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int number1;
    int number2;
} Shm;

#define MY_SHARED_FILE "/ex05_file"
#define MY_DATA_SIZE sizeof(Shm)

#define MY_SHM_OPEN_WRITER_OFLAG O_CREAT | O_EXCL | O_RDWR
#define MY_SHM_OPEN_WRITER_MODE S_IRUSR | S_IWUSR

#define MY_SHM_OPEN_READER_OFLAG O_EXCL | O_RDWR
#define MY_SHM_OPEN_READER_MODE S_IRUSR | S_IWUSR

#define MY_SHM_MAP_READ_WRITE PROT_READ | PROT_WRITE
#endif /* PL03_H_ */
