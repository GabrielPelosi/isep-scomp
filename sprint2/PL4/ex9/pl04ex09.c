#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#define N_PROCESSES 2
#define N_SEMAFOROS 2
#define SEMAFORO_01 0
#define SEMAFORO_02 1


int criar_processos(int n) {
	pid_t pid;
	int i;
	for (i = 0; i < n; i++) {
		pid = fork();
		if (pid < 0)
			return -1;
		else if (pid == 0)
			return i + 1;
	}
	return 0;
}

void buy_chips(){
	printf("buy_chips!\n");
}
void buy_beer(){
	printf("buy beer!\n");
}
void eat_and_drink(){
	printf("eat and drink!\n");
}

int main(void) {

	/*
	 * Crio dois semaforos para fazer a sincronização dos processos, pois um unico semaforo causaria um deadlock.
	 * Crio os semaforos de forma mais otimizada.
	 */
	const char SEMAFOROS_NAME[N_SEMAFOROS][80] = {"SEM_01_ex9","SEM_02_ex9"};
	const int SEMAFOROS_INITIAL_VALUE[N_SEMAFOROS] = {1,0};
	sem_t *semaforos[N_SEMAFOROS];

	int i;
	for (i = 0; i < N_SEMAFOROS; i++) {
		if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644,
				SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED) {
			perror("Error at sem_open()!\n");
			exit(EXIT_FAILURE);
		}
	}




	/*
	 * Utilizo novamente um metodo que cria os precessos e atribui um id a cada um para assim, cada filho ter sua respectiva tarefa.
	 *
	 */
	int id = criar_processos(N_PROCESSES);



	/*
	 * Para sincronizar, foi incializado um semaforo com valor 1 e outr com valor 0.
	 * Ao iniciar o algoritmo, o processo com o id 2 irá obrigatoriamente esperar o processo com id 1 executar a função buy_chips por causa de seus valores iniciais citados antes,
	 * Após isso, é feita uma sincronização onde os processos só irao executar o metodo eat, quando os outros 2 foram executados.
	 *
	 */
	if(id == 1){
		sem_wait(semaforos[SEMAFORO_01]);
		buy_chips();
		sem_post(semaforos[SEMAFORO_02]);
		sem_wait(semaforos[SEMAFORO_01]);
		eat_and_drink();
		exit(1);
	}

	if(id == 2){
		sem_wait(semaforos[SEMAFORO_02]);
		buy_beer();
		sem_post(semaforos[SEMAFORO_01]);
		eat_and_drink();
		exit(1);
	}

	//Pai eseprando todos os filhos
	for (i = 0; i < N_PROCESSES; i++) {
		wait(NULL);
	}

	//Cicl para fechar todos os semaforos
	for (i = 0; i < N_SEMAFOROS; i++) {
		if (sem_close(semaforos[i]) == -1) {
			perror("Error at sem_close()!\n");
			exit(EXIT_FAILURE);
		}
	}



	//Cicl para remoer todos os semaforos do SO
	for (i = 0; i < N_SEMAFOROS; i++) {
		if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
			perror("Error at sem_unlink()!\n");
			printf("Error: \n");
			exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}
