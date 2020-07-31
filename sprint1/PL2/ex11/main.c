#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_SIZE 1000 

int maiorNum(int a, int b) {
	if(a >= b) {
		return a;
	} else
		return b;
}

int main(){

	const int ESCRITA = 1;
	const int LEITURA = 0;

	pid_t p;
	int i, status, numProcs = 5, numPipes = 6, numPai, numFilho, readNum, maior, num;
	int fd[numPipes][2];

	numPai = rand() % 500 + 1;
	printf("PID: %d = %d\n", getpid(), numPai);

	/* criar pipes */
	for(i = 0; i < numPipes; i++) {
		if(pipe(fd[i]) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}

	for(i=0;i<numProcs;i++){
		numFilho = rand() % 500 + 1;
		p = fork();
	
		if(p==0){ //Filho
			printf("PID: %d = %d\n", getpid(), numFilho);

			read(fd[i][LEITURA], &readNum, sizeof(readNum));
			close(fd[i][LEITURA]);/* fechar extremidade de leitura */
			num = maiorNum(numFilho, readNum);

			write(fd[i+1][ESCRITA], &num, sizeof(num));
			close(fd[i+1][ESCRITA]);/* fechar extremidade de leitura */
			exit(0);
		}

	}

	write(fd[0][ESCRITA], &numPai, sizeof(numPai));
	close(fd[0][ESCRITA]);/* fechar extremidade de escrita */
	wait(&status); /* esperar que processos filho acabem */
	read(fd[5][LEITURA], &maior, sizeof(maior));
	close(fd[5][LEITURA]);/* fechar extremidade de leitura */

	printf("Maior num = %d\n", maior);

	return 0;
}

