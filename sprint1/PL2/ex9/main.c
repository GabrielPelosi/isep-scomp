#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct {
	int customer_code;
	int product_code;
	int quantity;
} sale;

int main() {
	int i, j, x = 0, code, status;
	pid_t p;
	int numChild = 10, numPipes = 10;
	int fd[numPipes * 2];
	sale sales[50000];
	int products[50000];
	
	//preencher sales com números aleatórios
	for(i = 0; i < 50000; i++) {
		sales[i].customer_code = rand() % 500;
		sales[i].product_code = rand() % 1000;
		sales[i].quantity = rand() % 25;
	}
	
	//criar pipes
	for(i = 0; i < numPipes; i++) {
		if(pipe(fd + i * 2) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}
	
	//criar os processos filho
	for(i = 0; i < numChild; i++) {
		p = fork();
		if(p == 0) { 
			close(*(fd + i * 2)); //fecha extremidadede de leitura
			
			int minLim = 5000 * i;
			int maxLim = 5000 * i + 5000;
			for(j = minLim; j < maxLim; j++) {
				//se o produto foi vendido mais de 20 vezes numa venda,
				//guarda o seu código
				if(sales[j].quantity > 20) {
					//envia código do produto para o processo pai
					write(*(fd + i * 2 + 1), &sales[j].product_code, 4);
				}
			}
					
			close(*(fd + i * 2 + 1)); //fecha extremidade de escrita
			exit(0);
		} else if(p > 0) {
			close(*(fd + i * 2 + 1)); //fecha a extremidade de escrita
			
			while((read(*(fd + i * 2), &code, 4)) > 0) {
				products[x] = code; //guarda código do produto
				x++;
			}
			
			close(*(fd + i * 2)); //fecha a extremidade de leitura 
		}
	}
	
	wait(&status); //processo pai espera pelos processos filhos
	if(WIFEXITED(status)) {
		printf("Produtos vendidos mais de 20 vezes:\n");
		for(i = 0; i < x; i++) {
			if(i == x-1){
				printf("%d\n", products[i]); //imprime o código do último produto
			} else{
				printf("%d; ", products[i]); //imprime os códigos dos produtos
			}
		}
	}
			
	return 0;
}
