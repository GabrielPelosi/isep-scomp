/*
 ============================================================================
 Name        : pl03ex05.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "pl03.h"

int main(void) {
	int i;
	//Criando a memoria partilhada
	int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_WRITER_OFLAG,MY_SHM_OPEN_WRITER_MODE);

	if (fd == -1) {
		printf("Erro ao criar memoria partilhada, verificar codigo");
		return -1;
	}

	//Definindo o tamanho da memoria partilhada
	ftruncate(fd, MY_DATA_SIZE);

	//Criando o apontador para receber a endereço da memoria partilhada
	//mapeando a memoria partilhada

	Shm *shm_data_type = mmap(NULL, MY_DATA_SIZE, MY_SHM_MAP_READ_WRITE,
			MAP_SHARED, fd, 0);

	shm_data_type->number1 = 8000;
	shm_data_type->number2 = 200;

	pid_t pid = fork();

	if (pid == -1) {
		printf("Erro no fork");
		return -1;
	}

	if (pid > 0) {
		for (i = 0; i < 1000000; i++) {
			shm_data_type->number1 += 1;
			shm_data_type->number2 -= 1;
		}
	} else {
		for (i = 0; i < 1000000; i++) {
			shm_data_type->number1 -= 1;
			shm_data_type->number2 += 1;
		}

		munmap(shm_data_type, MY_DATA_SIZE);
		close(fd);
		exit(2);
	}



	wait(NULL);
	printf("number1: %d , number2: %d \n", shm_data_type->number1,
			shm_data_type->number2);
	if(shm_unlink(MY_SHARED_FILE) < 0){
		perror("close failed");
		return -1;
	}


	return 0;
}
