/*
 ============================================================================
 Name        : pl02ex06.c
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
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

int fork_filhos(int n) {
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

	const int ESCRITA = 1;
	const int LEITURA = 0;
	const int ARRAY_SIZE = 1000;
	int vec1[ARRAY_SIZE];
	int vec2[ARRAY_SIZE];

	time_t t; //for srand

	///* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand((unsigned) time(&t));

	/* initialize array with random numbers (rand(): stdlib.h) */
	int i;
	for (i = 0; i < ARRAY_SIZE; i++) {
		vec1[i] = rand() % (4);
	}
	for (i = 0; i < ARRAY_SIZE; i++) {
		vec2[i] = rand() % (4);
	}

	int fd[2];
	pipe(fd);

	int id_process = fork_filhos(5);

	if (id_process == 0) {
		close(fd[ESCRITA]);
		int final = 0;
		int tmp;
		for (i = 0; i < 5; i++) {
			int status;
			wait(&status);
			if (WIFEXITED(status)) {
				read(fd[LEITURA], &tmp, 4);
				final += tmp;
			}
		}
		printf("Final result is %d\n", final);
	}
	if (id_process > 0) {
		close(fd[LEITURA]);
		int tmp = 0;
		int start = id_process * 200;
		int finish = start + 200;
		int i;
		for (i = start; i < finish; i++) {
			tmp = tmp + vec1[i] + vec2[i];
		}
		write(fd[ESCRITA], &tmp, 4);
		close(fd[ESCRITA]);
	}

	return EXIT_SUCCESS;
}
