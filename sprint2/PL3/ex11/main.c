#include "ex11.h"

#define MAX_SIZE 1000
#define CHILD_NUM 10

int main(){
	
	int fd[2];
	/* Avisa caso dê erro a criar o pipe*/
	if (pipe(fd) == -1){
		printf("Erro a criar o pipe.\n");
		return 0;
	}
	
	/* Preenche o array com 1000 integers de 0 a 1000 */
	int array[MAX_SIZE], i;
	for(i = 0; i < MAX_SIZE; i++){
		array[i] = rand() % MAX_SIZE + 1;
	}
	
	/* Cria os processos filho */
	pid_t p;
	for(i = 0; i < CHILD_NUM; i++){
		p = fork();
		/* Avisa caso dê erro na criação do processo filho */
		if (p < 0){
			printf("Erro a criar o processo filho.\n");
			exit(0);
		}
		/* Processo filho */
		if (p == 0){
			/* Fecha a extremidade de leitura do pipe */
			close(fd[0]);
			/* Procura o maior dos valores para um determinado intervalo do array */
			int j, max = 0, limiteMin, limiteMax, intervalo = MAX_SIZE / CHILD_NUM;
			limiteMin = i * intervalo;
			limiteMax = limiteMin + intervalo;
			for(j = limiteMin; j < limiteMax; j++){
				if(array[j] > max)
					max = array[j];
			}
			/* Escreve o maior valor (max) no pipe */
			write(fd[1], &max, sizeof(int));
			/* Fecha a extremidade de escrita do pipe */
			close(fd[1]);
			exit(0);
		}
	}
	
	/* O pai espera que todos os filhos terminem */
	for(i = 0; i < CHILD_NUM; i++){
		wait(NULL);
	}
	
	/* Fecha a extremidade de escrita do pipe */
	close(fd[1]);
	
	/* Procura o maior valor da lista de maiores valores entre os filhos
	 * Imprime o valor de todos os filhos */
	int max = 0;
	for(i = 0; i < CHILD_NUM; i++){
		int temp;
		read(fd[0], &temp, sizeof(int));
		if(temp > max){
			max = temp;
		}
		printf("Valor Máximo do Filho %d: %d\n", i, temp);
	}
	printf("Maior valor de todos: %d\n", max);
	/* Fecha a extremidade de leitura do pipe */
	close(fd[0]);
	
	return 0;
}
