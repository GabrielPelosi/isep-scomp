/*
 ============================================================================
 Name        : pl02ex02b.c
 Author      : pelosi
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

int main(void) {
	const int ESCRITA = 1;
	const int LEITURA = 0;
	pid_t pid;
	int input_int;
	char input_str[4];

	int fd[2];
	struct s {
		int a;
		char b[4];
	};


	if(pipe(fd)){
		printf("Erro ao criar o pipe!");
		return -1;
	}
	pid = fork();
	if( pid == -1){
		printf("Erro ao criar o fork");
		return -1;
	}



	if(pid > 0){
		struct s struct1;
		close(fd[LEITURA]);
		printf("Digite o int e logo apos a string!\n");
		scanf("%d", &input_int);
		scanf("%s", input_str);
		struct1.a = input_int;
		strcpy(struct1.b, input_str);
		write(fd[ESCRITA], &struct1, sizeof(struct1));
		close(fd[ESCRITA]);
	}


	if(pid == 0){
		struct s struct1;
		close(fd[ESCRITA]);
		read(fd[LEITURA], &struct1, sizeof(struct1));
		printf("Valor do int e da String respectivamente: %d  %s \n", struct1.a, struct1.b);
		close(fd[LEITURA]);
		exit(1);
	}

	return 0;
}
