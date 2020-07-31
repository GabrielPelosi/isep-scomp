#include <stdio.h>
#include <string.h>
#include <unistd.h>	   //Include do fork
#include <sys/types.h> //Include do pid_t
#include <stdlib.h>	   //Include do exit

#define TAMANHO_DATA 5

int main(){
	int fd[6][2], i, j, indice_, exst;
	pid_t pid;
	int cod_barras;

	/* Estrutura Produto*/
	struct product{
		char nome[50];
		float preco;
		int cod_barras;
	};
	/* pipes */
	for(i = 0; i < 6; i++){
		if(pipe(fd[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	/* filhos */
	for(i = 0; i < 5; i++){
		pid = fork();
		if(pid == 0){
			/* fechar a parte de leitura do primeiro pipe */
			close(fd[0][0]);
			/* fechar a parte de escrita do pipe de 1 i+1*/
			close(fd[i+1][1]);
			/* input cod_barras */
			printf("Leitos %d - Digite o codigo de barras para a analise (1, 2, 3, 4 ou 5):\n", i+1);
			scanf("%d", &cod_barras);
			/* passar os dados dados (cod_barras e i) no primeiro pipe */
			write(fd[0][1], &cod_barras, sizeof(int));
			write(fd[0][1], &i, sizeof(int));
			/* receber os dados (product) no pipe de indice_ice i+1 */
			struct product prod;
			read(fd[i+1][0], &prod, sizeof(struct product));
			printf("Leitor %d - Nome: %s\nPreco: %f\n", i+1, prod.nome, prod.preco);
			/* fechar a parte de escrita do primeiro pipe */
			close(fd[0][1]);
			/* fechar a parte de leitura do pipe de 1 i+1*/
			close(fd[i+1][0]);
			exit(0);
		}
	}

	struct product base_dados_produtos[TAMANHO_DATA];
	/* Colocando produtos na base de dados */
	strcpy(base_dados_produtos[0].nome, "Guaraná");
	base_dados_produtos[0].preco = 1.5;
	base_dados_produtos[0].cod_barras = 1;
	strcpy(base_dados_produtos[1].nome, "Noz");
	base_dados_produtos[1].preco = 1.99;
	base_dados_produtos[1].cod_barras = 2;
	strcpy(base_dados_produtos[2].nome, "Biscoito");
	base_dados_produtos[2].preco = 2.20;
	base_dados_produtos[2].cod_barras = 3;
	strcpy(base_dados_produtos[3].nome, "Bisteca");
	base_dados_produtos[3].preco = 1.19;
	base_dados_produtos[3].cod_barras = 4;
	strcpy(base_dados_produtos[4].nome, "Macarrao");
	base_dados_produtos[4].preco = 3;
	base_dados_produtos[4].cod_barras = 5;

	/* Pai */
	for(i = 0; i < 5; i++){
		/* fechar a parte de escrita do primeiro pipe */
		close(fd[0][1]);
		/* fechar a parte de leitura do pipe de 1 i+1*/
		close(fd[i+1][0]);
		/* input cod_barras */
		read(fd[0][0], &cod_barras, sizeof(int));
		read(fd[0][0], &indice_, sizeof(int));
		/* procurar respectivo produto */

		exst = 0;

		for(j = 0; j < TAMANHO_DATA; j++){


			if(cod_barras == base_dados_produtos[j].cod_barras){
				/* responder o request com os dados (base_dados_produtos[j]) no pipe_ */
				write(fd[indice_+1][1], &base_dados_produtos[j], sizeof(struct product));
				exst = 1;
			}
		}


		if(exst == 0){
			struct product prod;
			strcpy(prod.nome, "produto não encontrado na base de dados.");
			prod.preco = 0;
			/* responder com string o request com os dados (prod) no pipe*/
			write(fd[indice_+1][1], &prod, sizeof(struct product));
		}
	}

	return 0;
}
