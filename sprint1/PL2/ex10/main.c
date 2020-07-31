#include <stdio.h>
#include <unistd.h>	   
#include <sys/types.h> 
#include <stdlib.h>	  
#include <stdlib.h>
#include <time.h>

int main(){
	pid_t p;
	int fd1[2], fd2[2], credit = 20, num = 1, betChild, betParent;
	
	//criar primeiro pipe (pai escreve, filho lê)
	pipe(fd1); 
	//criar segundo pipe (filho escreve, pai lê)
	pipe(fd2);
	//cria processo filho
	p = fork();
	if(p == 0){
		/* assegura que betChild e betParent não são sempre iguais,
		 * todas as vezes que se corre o programa */
		srand((unsigned) time(NULL) * getpid());
		//fecha a extremidade de escrita do primeiro pipe
		close(fd1[1]);
		//fecha a extremidade de leitura do segundo pipe
		close(fd2[0]);
		while(credit > 0){
			//lê num
			read(fd1[0], &num, sizeof(int));
			// se num for 1 continua a apostar, se for 0, termina o processo
			if(num == 1){
				betChild = 1 + rand() % 5; //gera número aleatório de 1 a 5
				//escreve betChild no segundo pipe
				write(fd2[1], &betChild, sizeof(int));
				//lê credit no primeiro pipe
				read(fd1[0], &credit, sizeof(int));
				printf("Credito atual: %d\n", credit);
			} else {
				//fecha a extremidade de leitura do primeiro pipe
				close(fd1[0]);
				//fecha a extremidade de escrita do segundo pipe
				close(fd2[1]);
				exit(0);
			}
		}
	} else {
		/* assegura que betChild e betParent não são sempre iguais,
		 * todas as vezes que se corre o programa */
		srand((unsigned) time(NULL) * getpid()); 
		//fecha a extremidade de leitura do primeiro pipe
		close(fd1[0]);
		//fecha a extremidade de escrita do segundo pipe
		close(fd2[1]);
		while(credit > 0){
			betParent = 1 + rand() % 5; //gera número aleatório de 1 a 5
			if(credit > 0){
				num = 1; 
			} else {
				num = 0;
			}
			//escreve num no primeiro pipe
			write(fd1[1], &num, sizeof(int));
			//lê betChild do segundo pipe
			read(fd2[0], &betChild, sizeof(int));
			printf("Aposta Filho: %d; Resultado Pai: %d\n", betChild, betParent);
			if(betChild == betParent){
				credit = credit + 10; //acrescenta 10 se ganhar a aposta
			} else {
				credit = credit - 5; //subtrai 5 se perder a aposta
			}
			//escreve credit no primeiro pipe
			write(fd1[1], &credit, sizeof(int));
		}
		//fecha a extremidade de escrita do primeiro pipe
		close(fd1[1]);
		//fecha a extremidade de leitura do segundo pipe
		close(fd2[0]);
	}
	return 0;
}
