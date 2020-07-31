#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
	int vec1[1000], vec2[1000], fd[5][2], res[1000];
	pid_t pid;
	int i, j, x, total = 0, soma = 0, totaL = 0;

	/*Preenchimento dos vectores*/
	for (i = 0; i < 1000; ++i)	{
		vec1[i] = 1;
		vec2[i] = i;
	}

	for (i = 0; i < 5; i++)	{
		if (pipe(fd[i]) == -1)	{	
			printf("Erro no pipe!\n");
			return 0;
		}
		
		pid = fork();

		if (pid == -1)	{
			printf("Erro no fork!\n");
			return 0;
		}

		if (pid == 0)	{
			close(fd[i][0]);
			for (j = (i*200); j < 200 + (i*200); j++){
				total = vec1[j] + vec2[j];
				write(fd[i][1], &total, sizeof(int));
			}
			close(fd[i][1]);
			exit(1);
		} else { 
			wait(&x);	
			close(fd[i][1]);
			for (j = (i*200); j < 200 + (i*200); j++){

				read(fd[i][0], &soma, sizeof(int));
				res[i] = soma;
				printf("Total: %d\n", res[i]);	
			}
			printf("\n");
		}
		close(fd[i][0]);		
	}
}

