/*
 ============================================================================
 Name        : pl04ex16East.c
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

	sem_t *sem_east, *sem_ponte;
	if ((sem_east = sem_open("sem_east", O_EXCL, 0644, 0)) == SEM_FAILED || (sem_ponte = sem_open("sem_ponte", O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}

	/* incrementar o semaforo sem_east, ou seja, significa que mais um carro chegou */
	sem_post(sem_east);
	/* Se o carro chegar primeiro em east,passa, se ja estiver um carro em west, esse tem que esperar que o outro passe.
	 * isso acontece pois o semaforo da ponte inicia em 1
	 * e é descontado esse valor no primeiro carro que passa, independente do lado, ou seja,
	 * se a ponte ja estiver com um carro seu valor ja será 0 no semaforo, e o carro do outro lado irá esperar no sem wait dentro do if,
	 * pois terá o contador igual a 1 juntamente com o contador do semaforo da ponte igual a 0.
	 */
	int value;
	sem_getvalue(sem_east, &value);
	printf("East: carro %d acabou de chegar\n", value);
	if(value == 1)
		sem_wait(sem_ponte);
	/* duração do tajeto da ponte */
	printf("East: carro %d está passando\n", value);
	sleep(30);
	/* decrementar sem_east, significa que saiu da ponte */
	printf("East: carro %d está saindo\n", value);
	sem_wait(sem_east);
	/* após passar da ponte, é incrementado um valor no semaforo da ponte, para dizer que o carro de west pode passar */
	sem_getvalue(sem_east, &value);
	if(value == 0)
		sem_post(sem_ponte);

	return 0;
}
