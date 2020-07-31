/*
 ============================================================================
 Name        : pl04ex07.c
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
#define N_PROCESSES 3
#define N_SEMAFOROS 3
#define SEMAFORO_01 0
#define SEMAFORO_02 1
#define SEMAFORO_03 2

int criar_processos(int n) {
	pid_t pid;
	int i;
	for (i = 0; i < n; i++) {
		pid = fork();
		if (pid < 0)
			return -1;
		else if (pid == 0)
			return i + 1;
	}
	return 0;
}

int main(void) {

	/*
	 *
	  Crio os 3 semaforos necessário em um array de semaforos, cada um com um respectivo nome e valor inicial.
	  Semaforos iniciados de uma forma mais simples.
	  São necessário 3 semaforos.Com semaforos a menos, o algoritmo, ou entrava em deadlock, ou não era capaz de resolver
	  o problema proposto pelo enunciado.
	  O primeiro semaforo é iniciado com 1 para pode permitir que a primeira frase seja imprimida sem problemas.
	 */
	const char SEMAFOROS_NAME[N_SEMAFOROS][80] = { "SEM_01_ex7", "SEM_02_ex7",
			"SEM_03_ex7" };
	const int SEMAFOROS_INITIAL_VALUE[N_SEMAFOROS] = { 1, 0, 0 };
	sem_t *semaforos[N_SEMAFOROS];

	int i;
	for (i = 0; i < N_SEMAFOROS; i++) {
		if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644,
				SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED) {
			perror("Error at sem_open()!\n");
			exit(EXIT_FAILURE);
		}
	}

	//Crio os processos filhos em um metodo separado para atribuir um id a cada processo e assim,
	//poder dividir uma tarefa para cada processo
	int id = criar_processos(N_PROCESSES);

	/*
	 * Nessa parte foi dividida as tarefas e foi necessário adicionar
	 * um \n nos printf para que o out put saisse organizado
	 */
	if (id == 1) {
		sem_wait(semaforos[SEMAFORO_01]);
		printf("Sistemas \n");
		sem_post(semaforos[SEMAFORO_02]);

		sem_wait(semaforos[SEMAFORO_01]);
		printf("a \n");
		sem_post(semaforos[SEMAFORO_02]);
		exit(1);
	}
	if (id == 2) {
		sem_wait(semaforos[SEMAFORO_02]);
		printf("de \n");
		sem_post(semaforos[SEMAFORO_03]);

		sem_wait(semaforos[SEMAFORO_02]);
		printf("melhor \n");
		sem_post(semaforos[SEMAFORO_03]);
		exit(2);
	}

	if (id == 3) {
		sem_wait(semaforos[SEMAFORO_03]);
		printf("Computadores - \n");
		sem_post(semaforos[SEMAFORO_01]);

		sem_wait(semaforos[SEMAFORO_03]);
		printf("disciplina! \n");
		exit(3);
	}

	//Pai eseprando todos os filhos
	for (i = 0; i < N_PROCESSES; i++) {
		wait(NULL);
	}

	//Cicl para fechar todos os semaforos
	for (i = 0; i < N_SEMAFOROS; i++) {
		if (sem_close(semaforos[i]) == -1) {
			perror("Error at sem_close()!\n");
			exit(EXIT_FAILURE);
		}
	}

	//Cicl para remoer todos os semaforos do SO
	for (i = 0; i < N_SEMAFOROS; i++) {
		if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
			perror("Error at sem_unlink()!\n");
			printf("Error: \n");
			exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}
