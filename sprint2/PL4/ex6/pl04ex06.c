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
	const int NUM_REPETICOES = 15;
	sem_t *sem2;
	//Criação do semaphore para sincronizar
	if ((sem2 = sem_open("/sem_ex06", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}

	//loop para realizar as repeticoes
	//o filho faz o print direto sem interferir no contador do semafaro, enquanto o pai, desconta um valor, logo o filho
	//sempre vai fazer o print primeiro visto que o contador comeca em zero, apos o print é incrementado um valor
	int i;
	pid_t pid;
	for (i = 0; i < NUM_REPETICOES; i++) {
		pid = fork();
		if (pid == 0) {
			printf("Eu sou o filho\n");
			sem_post(sem2);
			exit(1);
		}
		if (pid > 0) {
			sem_wait(sem2);
			printf("Eu sou o pai\n");
		}

	}

	//close e delete do semafaro
	sem_close(sem2);
	sem_unlink("/sem_ex06");

	return EXIT_SUCCESS;
}
