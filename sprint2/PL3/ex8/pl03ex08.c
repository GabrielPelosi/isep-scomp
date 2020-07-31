/*
 ============================================================================
 Name        : pl03ex08.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "pl03.h"

int main(void) {;
    const int VALOR_INICIAL = 100;
    const int N_OPERACOES = 1000000;

    int fd = shm_open(MY_SHARED_FILE, /*O_EXCL |*/ MY_SHM_OPEN_WRITER_OFLAG, MY_SHM_OPEN_WRITER_MODE);
    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }


    ftruncate(fd, MY_DATA_SIZE);


    Shm *shm_data = mmap(NULL, MY_DATA_SIZE, MY_SHM_MAP_READ_WRITE, MAP_SHARED, fd, 0);


    shm_data->number1 = VALOR_INICIAL;

    //fork
    pid_t pid = fork();

    int i;
    for (i = 0; i < N_OPERACOES; i++) {
        shm_data->number1 += 1;
        shm_data->number1 -= 1;
    }

    if (pid > 0) {
        printf("Pai : %d\n", shm_data->number1);

        wait(NULL);

        //eliminando shm
        if (shm_unlink(MY_SHARED_FILE) < 0) {
            printf("Error at shm_unlink()!\n");
            exit(EXIT_FAILURE);
        }
    }

    if (pid == 0) {
        printf("Filho : %d\n", shm_data->number1);
    }

    //desconectando mapa
    if (munmap(shm_data, MY_DATA_SIZE) < 0) {
        printf("Error  munmap()!\n");
        exit(EXIT_FAILURE);
    }

    //fechando fd
    if (close(fd) < 0) {
        printf("Error  close()!\n");
        exit(EXIT_FAILURE);
    }

	return EXIT_SUCCESS;
}
