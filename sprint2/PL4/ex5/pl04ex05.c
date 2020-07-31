/*
 ============================================================================
 Name        : pl04ex05.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

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



int main(void) {
	sem_t *sem2;
	//Criação do semaphore
	if ((sem2 = sem_open("/sem_ex05_v2", O_CREAT | O_EXCL, 0644, 0))
			== SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}

	//Como o filho faz o print sem interferir no contador
	//o pai sempre ficará esperando seu print para que ele incrmente
	pid_t pid = fork();

	if (pid ==0){
		printf("Eu sou o filho\n");
		sem_post(sem2);
	}
	if(pid > 0){
		sem_wait(sem2);
		printf("Eu sou o pai\n");
	}

	sem_close(sem2);
	sem_unlink("/sem_ex05_v2");


	return EXIT_SUCCESS;
}
