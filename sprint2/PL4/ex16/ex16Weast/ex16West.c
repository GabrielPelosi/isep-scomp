/*
 ============================================================================
 Name        : pl04ex16West.c
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
	/* Criar os semáforos */
	sem_t *sem_west, *sem_ponte;
	if ((sem_west = sem_open("sem_west", O_EXCL, 0644, 0)) == SEM_FAILED || (sem_ponte = sem_open("sem_ponte", O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}

	/* Incrementar semWest - significa que mais um carro chegou de West */
	sem_post(sem_west);
	/*
	 * Logica igual a do East, o carro do West so passa se for o primeiro a chegar, se ja tiver carro na ponte vindo de
	 * east, tem que eseprar(dentro do if) o carro terminar a ponte e incrmentar o semaforo da ponte para 1.
	 */
	int value;
	sem_getvalue(sem_west, &value);
	printf("West: carro%d chegou\n", value);
	if(value == 1)
		sem_wait(sem_ponte);
	/* Demora 30s a passar a ponte */
	printf("West: carro%d a passar\n", value);
	sleep(30);
	/* Decrementar sem_west - significa que saiu da ponte */
	printf("West: carro%d a sair\n", value);
	sem_wait(sem_west);
	/* Quando acaba de passar na ponte, incrementa o semaforo da ponte para caso haja carros emeast quendeo passar, tenham seu caminho liberado */
	sem_getvalue(sem_west, &value);
	if(value == 0)
		sem_post(sem_ponte);

	return 0;
}
