/*
 ============================================================================
 Name        : pl03ex03Writer.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "pl03.h"


int main(void) {

		int numbers[ARRAY_SIZE];

	    time_t t; //for srand

	    ///* intializes RNG (srand():stdlib.h; time(): time.h) */
	    srand((unsigned)time(&t));

	    /* initialize array with random numbers (rand(): stdlib.h) */
	    int i;
	    for (i = 0; i < ARRAY_SIZE; i++)
	    {
	        numbers[i] = rand() % 18;
	    }
	    /* initialize n */

	//criação da memoria partilhada
	int fd = shm_open(MY_SHARED_FILE,MY_SHM_OPEN_WRITER_OFLAG, MY_SHM_OPEN_WRITER_MODE);

	if(fd == -1){
		printf("Erro ao criar SHM, rever o codigo!");
		return -1;
	}

	//definindo o tamanho
	ftruncate(fd, MY_DATA_SIZE);


	//mapeamento da memoria partilhada
	Array *sharedMemory = mmap(NULL,MY_DATA_SIZE,MY_SHM_MAP_READ_WRITE, MAP_SHARED, fd , 0);

	for(i = 0; i<ARRAY_SIZE;i++){
		sharedMemory->num[i] = numbers[i];
		printf("Numero aleatorio: %d\n", sharedMemory->num[i]);
	}


	return EXIT_SUCCESS;
}
