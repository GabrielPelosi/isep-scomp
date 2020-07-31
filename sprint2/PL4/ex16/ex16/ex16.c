/*
 ============================================================================
 Name        : pl04ex16.c
 Author      : pelosi
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

int main(){
	//essa aplicacao foi criada para criar os semaforos separadamentes, assim fica mais organizado e mais
	//simples de fazer novas versoes.
	sem_unlink("sem_east");
	sem_unlink("sem_ponte");
	sem_unlink("sem_west");

	/*cria semáforos */
	sem_t *sem_west, *sem_ponte, *sem_east;
	if ((sem_west = sem_open("sem_west", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ||
	(sem_east = sem_open("sem_east", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ||
	(sem_ponte = sem_open("sem_ponte", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}

	return 0;
}
