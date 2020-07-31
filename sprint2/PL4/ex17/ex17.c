/*
 ============================================================================
 Name        : pl04ex17.c
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

#define NUM_FILHOS 305 /* número de processos filho */
#define MAX_VISITANTES 300 /* quantidade máxima de visitantes */

#define ID_VIP 0 /* id visitante VIP */
#define ID_ESPECIAL 1 /* id Special */
#define ID_NORMAL 2 /* id visitante Normal */

#define SEM_NAME_1 "semaforo1" /* primeiro sem */
#define SEM_NAME_2 "semaforo2" /* segundo sem */
#define SEM_NAME_3 "semaforo3" /* terceiro sem */
#define SEM_NAME_4 "semaforo4" /*quarto sem */

int main() {

	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	sem_unlink(SEM_NAME_3);
	sem_unlink(SEM_NAME_4);

	int i, id, n_vip = 0, num_especial = 0;

	sem_t *sem_vip; /* limita entrada de visitantes VIP */
	sem_t *sem_especial; /* limita a entrada de visitantes Special */
	sem_t *sem_normal; /* limita a entrada de visitantes Normal */
	sem_t *sem_visitantes; /* limita a capacidade do cinema */


	if((sem_vip = sem_open(SEM_NAME_1, O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_especial = sem_open(SEM_NAME_2, O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_normal = sem_open(SEM_NAME_3, O_CREAT, 0644, 0)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_visitantes = sem_open(SEM_NAME_4, O_CREAT, 0644, MAX_VISITANTES)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}

	for(i = 0; i < NUM_FILHOS; i++) {
		if(i % 3 == ID_VIP)
			n_vip++; /* aumenta os VIPs na fila de espera */
		if(i % 3 == ID_ESPECIAL)
			num_especial++; /* aumenta os especias na fila de espera aumenta */
	}

	for(i = 0; i < NUM_FILHOS; i++) {
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(pid == 0) {
			while(1) {
				id = i % 3; /* atribui uma classificação ao visitante VIP, Special ou Normal */

				if(id == ID_VIP) { /* se o visitante for VIP */
					sem_post(sem_vip);
					sem_wait(sem_vip); /* entrada de um VIP */
					sem_wait(sem_visitantes); /*numero de lugares vazios diminui */

					printf("Visitante VIP %d entrou\n", i + 1);
					n_vip = n_vip - 1; /* quantidade de VIPs diminui na fila de espera */

				} else if(id ==ID_ESPECIAL) { /* se o visitante for especial */
					sem_post(sem_especial);
					sem_wait(sem_especial); /* entrada de um especial */
					sem_wait(sem_visitantes); /* numero de lugares vazios diminui */

					printf("Visitante Special %d entrou\n", i + 1);
					num_especial--; /* número de especias na fila de espera diminui */

				} else { /* se o visitante for Normal */
					sem_post(sem_normal);
					sem_wait(sem_normal); /* entrada de um visitante Normal */
					sem_wait(sem_visitantes); /* numero de lugares vazios diminui */
					printf("Visitante Normal %d entrou\n", i + 1);
				}

				sleep(5); /* tempo que os visitantes ficam no cinema assisitndo */

				sem_post(sem_visitantes); /* auementa quantiadde de lugares vagos */
				printf("Visitante %d saiu\n", i + 1);

				if(n_vip > 0) { /* VIPs entram em primeiro lugar */
					sem_post(sem_vip);
				} else if(n_vip <= 0 && num_especial > 0) { /* especiais entram em segundo lugar */
					sem_post(sem_especial);
				} else if(n_vip <= 0 && num_especial <= 0) {
					sem_post(sem_normal); /* Normais entram em último lugar */
				}
				exit(0);
			}
		}
	}

	for(i = 0; i < NUM_FILHOS; i++)
		wait(NULL);

	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	sem_unlink(SEM_NAME_3);
	sem_unlink(SEM_NAME_4);

	return 0;
}
