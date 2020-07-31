/*
 ============================================================================
 Name        : pl04ex15.c
 Author      : pelosi
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>

#define NUM_FILHOS 5 // número de filhos
#define MAX_VISITANTES 5 // quantidade máxima de visitantes
#define WAITING_TIME 2 // tempo de espera
#define DURACAO_SHOW 5 // tempo que o show dura

#define SEM_NAME_1 "semaforo1" //  primeiro semáforo
#define SEM_NAME_2 "semaforo2" // segundo semáforo
#define SEM_NAME_3 "semaforo3" // terceiro semáforo
#define SEM_NAME_4 "semaforo4" //  quarto semáforo

int main() {
	/* fechar os semáforos para evitar erros com semaforos ja criados*/
	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	sem_unlink(SEM_NAME_3);
	sem_unlink(SEM_NAME_4);

	int i;
	srand(time(NULL));

	sem_t *sem_show; /* controla o início do espetáculo */
	sem_t *sem_visitantes; /* controla a quantidade de lugares vagos na sala */
	sem_t *sem_show_entrada; /* controla a entrada na sala */
	sem_t *sem_show_saida; /* controla a saída da sala */

	/* criar semáforos */
	if((sem_show = sem_open(SEM_NAME_1, O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_visitantes = sem_open(SEM_NAME_2, O_CREAT, 0644, MAX_VISITANTES)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_show_entrada = sem_open(SEM_NAME_3, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_show_saida = sem_open(SEM_NAME_4, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}

	/*
	 * A logico foi feita em um loop infinito que possui um exit para os filhos.
	 * A sincronização foi feita com 4 semaforos,um para esperar, entrar , assistir e sair.
	 * As ações acontecem uma de cada vez por cada visitante, ou seja, o primeiro visitante espera, entra e assiste.
	 * Para sair também, eles saem um de cada vez.
	 */
	for(i = 0; i < NUM_FILHOS; i++) {
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(pid == 0) {
			while(1) {

				sem_wait(sem_show_entrada);
				sem_post(sem_show_entrada); /* sem_post para o próximo visitante entrar */
				printf("Visitante %d à espera\n", i + 1);

				if((i + 1) == MAX_VISITANTES) { /* se a sala estiver cheia o show comeaa */
					sem_post(sem_show);
				}

				sem_wait(sem_visitantes); /* visitante entra na sala */
				printf("Visitante %d entrou\n", i + 1);

				sem_wait(sem_show);
				sem_post(sem_show); /* sen_post  para o próximo visitante assistir */
				printf("Visitante %d a assistir\n", i + 1);

				sleep(DURACAO_SHOW); /* duracao do show */

				sem_wait(sem_show_saida);
				sem_post(sem_show_saida);
				sem_post(sem_visitantes); /* lugares vagos na sala aumenta */
				printf("Visitante %d saiu\n", i + 1);

				exit(0);
			}
		}
	}

	for(i = 0; i < NUM_FILHOS; i++)
		wait(NULL);

	/* fechar os semáforos */
	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	sem_unlink(SEM_NAME_3);
	sem_unlink(SEM_NAME_4);

	return 0;
}
