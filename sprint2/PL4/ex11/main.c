#include<stdio.h>
#include<time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdbool.h>
#define NUM_CHILDS 3 /* número de processos filho */
#define NUM_PASSENGERS 200 /* número máximo de passageiros */
#define SEM_NAME_1 "sem1" /* nome do primeiro semáforo */
#define SEM_NAME_2 "sem2" /* nome do segundo semáforo */
#define SEM_NAME_3 "sem3" /* nome do terceiro semáforo */
#define SEM_NAME_4 "sem4" /* nome do quarto semáforo */


int main(void){
    int i, j, entered, left, capacityLeft;
	sem_t *sem_porta1, *sem_porta2, *sem_porta3, *sem_pass;
	
	/* criar semáforos */
	if((sem_porta1 = sem_open(SEM_NAME_1, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_porta2 = sem_open(SEM_NAME_2, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_porta3 = sem_open(SEM_NAME_3, O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_pass = sem_open(SEM_NAME_4, O_CREAT, 0644, 200)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}

    for(i = 0; i < NUM_PASSENGERS; i++) { /* para cada passageiro */
		pid_t pid = fork();
		if(pid == -1) {
			perror("Failed fork\n");
			exit(0);
		} else if(pid == 0) {
			
			/* simulação de entrada no comboio */
			
			entered = 0; /* variável que indica se alguém já entrou no comboio */
			while(entered == 0) { /* enquanto ninguém entrou */
				for(j = 0; j < NUM_CHILDS; j++) { /* para cada porta */
				
					if(sem_trywait(sem_porta1) != 0) { /* avançar se porta 1 não estiver bloqueada */
						
						sem_wait(sem_pass); /* diminuir capacidade restante do comboio */
						entered = 1; /* indicação que alguém entrou */
						
						/* imprimir capacidade restante */
						sem_getvalue(sem_pass, &capacityLeft);
						printf("Passageiro entrou pela porta 1 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(sem_porta1); /* desbloquear porta 1 */
						break;
					}
					
					if(sem_trywait(sem_porta2) != 0) { /* avançar se porta 2 não estiver bloqueada */
						
						sem_wait(sem_pass); /* diminuir capacidade restante do comboio */
						entered = 1; /* indicação que alguém entrou */
						
						/* imprimir capacidade restante */
						sem_getvalue(sem_pass, &capacityLeft);
						printf("Passageiro entrou pela porta 2 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(sem_porta2); /* desbloquear porta 2 */
						break;
					}

					if(sem_trywait(sem_porta3) != 0) { /* avançar se porta 3 não estiver bloqueada */
						
						sem_wait(sem_pass); /* diminuir capacidade restante do comboio */
						entered = 1; /* indicação que alguém entrou */
						
						/* imprimir capacidade restante */
						sem_getvalue(sem_pass, &capacityLeft);
						printf("Passageiro entrou pela porta 3 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(sem_porta3); /* desbloquear porta 3 */
						break;
					}
				}
			}
			
			/* simulação de saída do comboio */
			
			left = 0; /* variável que indica se alguém já saiu do comboio */
			while(left == 0) { /* enquanto ninguém saiu */
				for(j = 0; j < NUM_CHILDS; j++) { /* para cada porta */
				
					if(sem_trywait(sem_porta1) != 0) { /* avançar se porta 1 não estiver bloqueada */
											
						sem_post(sem_pass); /* aumentar capacidade restante do comboio */ 
						left = 1; /* indicação que alguém saiu */
						
						/* imprimir capacidade restante */
						sem_getvalue(sem_pass, &capacityLeft);
						printf("Passageiro saiu pela porta 1 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(sem_porta1); /* desbloquear porta 1 */
						break;
					}
					
					if(sem_trywait(sem_porta2) != 0) { /* avançar se porta 2 não estiver bloqueada */
											
						sem_post(sem_pass); /* aumentar capacidade restante do comboio */ 
						left = 1; /* indicação que alguém saiu */
						
						/* imprimir capacidade restante */
						sem_getvalue(sem_pass, &capacityLeft);
						printf("Passageiro saiu pela porta 2 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(sem_porta2); /* desbloquear porta 2 */
						break;
					}
					
					if(sem_trywait(sem_porta3) != 0) { /* avançar se porta 3 não estiver bloqueada */
											
						sem_post(sem_pass); /* aumentar capacidade restante do comboio */ 
						left = 1; /* indicação que alguém saiu */
						
						/* imprimir capacidade restante */
						sem_getvalue(sem_pass, &capacityLeft);
						printf("Passageiro saiu pela porta 3 - Capacidade Restante %d\n", capacityLeft);
						
						sem_post(sem_porta3); /* desbloquear porta 3 */
						break;
					}
				}
			}
			exit(0);
		}
	}
	
	/* processo pai espera que processos filho acabem */
	for(i = 0; i < NUM_PASSENGERS; i++)
		wait(NULL);
	
	/* fechar os semáforos */
	sem_unlink(SEM_NAME_1);
	sem_unlink(SEM_NAME_2);
	sem_unlink(SEM_NAME_3);
	sem_unlink(SEM_NAME_4);

    return 0;

}
