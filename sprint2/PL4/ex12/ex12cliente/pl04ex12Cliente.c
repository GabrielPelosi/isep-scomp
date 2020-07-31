#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "includes.h"


#define NUMBER_OF_SEMAFOROS 2

typedef struct {
    int clientes;
    int bilhetes;
} shared_data;

int main(){


    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"SEM_01", "SEM_02"};

    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {0,1};
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    //inicializar semaforos
    int i;
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++)
    {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error at sem_open()cliente!\n");
            exit(EXIT_FAILURE);
        }
    }


    shared_data* shared_data1;
    int fd;
    				/* OPEN SHARED MEMORY*/
	if((fd = shm_open("/ex12shm", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR)) == -1){
		perror("shm_open error");
		exit(1);
	}

		/* TUNCATE SHARED MEMORY SIZE */
	if(ftruncate(fd,sizeof(shared_data1)) == -1){
		perror("ftruncate error");
		exit(1);
	}

	/* Map da shared memory */
	if(( shared_data1 = (shared_data*) mmap(NULL, sizeof(shared_data1), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

	/*
	 * Faço um loop que percorre o um valor igual ao numero de clientes, para cada cliente fazer o print do
	 * seu bilhete.o sem wait é para esperar o vendedor vender e o sempost é para avisar ao vendedor queo cliente
	 * ja foi atentido e o prox cliente pode ser atnedido, isso acontece para cada cliente
	 */
	int count = 0;
	do{
		shared_data1->clientes++;
		__semWAIT(semaforos[0]);
    	printf("O numero do meu bilhete é %d\n", shared_data1->bilhetes);
    	__semPOST(semaforos[1]);
    	count++;
	}while(count != NUMERO_CLIENTES);



    return 0;
}
