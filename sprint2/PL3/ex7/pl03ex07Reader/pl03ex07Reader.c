/*
 ============================================================================
 Name        : pl03ex07Reader.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "pl03.h"

int main(void) {
	int fd = shm_open(MY_SHARED_FILE,MY_SHM_OPEN_READER_OFLAG, MY_SHM_OPEN_READER_MODE);

		if (fd == -1) {
			printf("Erro ao crair memoria partilhada, rever o codigo");
			return -1;
		}

		ftruncate(fd,MY_DATA_SIZE);

		Array *sharedData;
		sharedData = mmap(NULL,MY_DATA_SIZE, MY_SHM_MAP_READ_WRITE, MAP_SHARED, fd ,0);


		int i;
		int total_num = 0;
		for(i = 0; i< ARRAY_SIZE;i++){
			total_num +=sharedData->num[i];
			printf("Student Number: %d\n", sharedData->num[i]);
		}

		printf("Média dos valores do array: %.2f", (double) (total_num/ARRAY_SIZE));

	return EXIT_SUCCESS;
}
