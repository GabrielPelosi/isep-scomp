#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_SIZE 1000 
#define M1_M2 "5 peças prontas de M1 para M2.\n"
#define M2_M3 "5 peças prontas de M2 para M3.\n"
#define M3_M4 "10 peças prontas de M3 para M4.\n"
#define M4_A1 "100 peças prontas para A1.\n"

int main(){

	const int ESCRITA = 1;
	const int LEITURA = 0;

	pid_t p;
	int i, x = 0, k=0, status, numPipes = 4, numMensgM1, numPecas = 1000;
	int fd[numPipes][2];
	char leituraM2[MAX_SIZE];
	char leituraM3[MAX_SIZE];
	char leituraM4[MAX_SIZE];
	char leituraA1[MAX_SIZE];

	/* criar pipes */
	for(i = 0; i < numPipes; i++) {
		if(pipe(fd[i]) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}

	numMensgM1 = numPecas/5;

	p = fork();
	if(p==0){ //Máquina 1
		p = fork(); //Máquina 2
		if(p>0){ //Máquina 1
			for(i=0;i<numMensgM1;i++){

				write(fd[k][ESCRITA], &M1_M2, strlen(M1_M2)+1);
				printf("%s\n", M1_M2);
				close(fd[k][ESCRITA]); 
			}
		}else{
			p = fork(); //Máquina 3
			if(p>0){ //Máquina 2
				read(fd[k][LEITURA], &leituraM2, strlen(leituraM2)+1);
				printf("Maquina 2 notificação: %s\n", leituraM2);
				close(fd[k][LEITURA]);
				write(fd[k+1][ESCRITA], &M2_M3, strlen(M2_M3)+1);
				printf("%s\n", M2_M3);
				close(fd[k+1][ESCRITA]); 
			}else{
				p = fork(); //Máquina 4
				if(p>0){//Máquina 3
					read(fd[k+1][LEITURA], &leituraM3, strlen(leituraM3)+1);
					printf("Maquina 3 notificação: %s\n", leituraM3);
					close(fd[k+1][LEITURA]);
					x++;
					if(x == 2){
						write(fd[k+2][ESCRITA], &M3_M4, strlen(M3_M4)+1);
						printf("%s\n", M3_M4);
						close(fd[k+2][ESCRITA]);
						x = 0;
					}
				}else{
					read(fd[k+2][LEITURA], &leituraM4, strlen(leituraM4)+1);
					printf("Maquina 4 notificação: %s\n", leituraM4);
					close(fd[k+2][LEITURA]);
					x++;
					if(x == 10){
						write(fd[k+3][ESCRITA], &M4_A1, strlen(M4_A1)+1);
						printf("%s\n", M4_A1);
						close(fd[k+3][ESCRITA]);
						x = 0;
					}
				} 
			}	
		}
		
	}else{
		wait(&status);
		read(fd[k+3][LEITURA], &leituraA1, strlen(leituraA1)+1);
		printf("Armazém 1 notificação: %s\n", leituraA1);
		close(fd[k+3][LEITURA]);
		x++;
		if(x == 10){
			printf("Fim de Produção.\n");
		}
	}
	return 0;
}

