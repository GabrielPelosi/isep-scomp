/*
 ============================================================================
 Name        : pl02ex03.c
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
	const int ESCRITA =1;
	const int LEITURA = 0;
	int num;
	char str[10];

	int fd[2];
	if(pipe(fd) == -1){
		printf("Erro no pipe!");
		return -1;
	}
	pid_t pid = fork();
	if(pid ==-1){
		printf("Erro no fork!");
		return -1;
	}

	if(pid > 0){
		close(fd[LEITURA]);
		printf("Digite o int");
		scanf("%d", &num);
		write(fd[ESCRITA], &num, sizeof(num));
		printf("Digite a string!");
		scanf("%s", str);
		write(fd[ESCRITA], str,sizeof(str));
		close(fd[ESCRITA]);
	}

	if(pid == 0){
		close(fd[ESCRITA]);

		read(fd[LEITURA],&num,sizeof(num));
		printf("print do int filho: %d\n", num);
		read(fd[LEITURA],str,sizeof(str));
		printf("print do str filho: %s\n", str);
		close(fd[LEITURA]);
		exit(1);
	}

	return 0;
}
