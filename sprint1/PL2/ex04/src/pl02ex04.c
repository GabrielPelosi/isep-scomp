/*
 ============================================================================
 Name        : pl02ex04.c
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

int main(void) {
	const int ESCRITA = 1;
	const int LEITURA = 0;
	const char* ficheiro = "ficheiro.txt.txt";
	int fd[2];
	FILE *fp;
	ssize_t read_s;
	size_t tam = 0;

	char *linha = NULL;

	if(pipe(fd) == -1){
		return -1;
	}

	pid_t pid = fork();

	if(pid ==-1){
		return -1;
	}

	if(pid > 0){
		close(fd[LEITURA]);
		fp = fopen(ficheiro, "r");
		if(fp == NULL)
			exit(EXIT_FAILURE);
		while ((read_s = getline(&linha, &tam, fp)) != -1){
			write(fd[ESCRITA], linha, read_s);
		}
		fclose(fp);
		close(fd[ESCRITA]);
		if(linha!=NULL)
			free(linha);
		wait(NULL);
	}


	if(pid == 0){
		close(fd[ESCRITA]);
		char data[80];
		while(read(fd[LEITURA], data, sizeof(data))){
			printf("%s", data);
		}
		close(fd[LEITURA]);
		exit(255);
	}

	return EXIT_SUCCESS;
}
