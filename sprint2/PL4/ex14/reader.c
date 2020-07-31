#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h> 
#include <time.h>
#include <semaphore.h>
#include "structs.h"

int main(){
	/* Abrir memória partilhada */
	int fdS = shm_open("/shmEx14_Strings", O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	int fdR = shm_open("/shmEx14_Readers", O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	/* Caso dê erro a abrir a memória partilhada */
	if (fdS == -1 || fdR == -1){
		perror("Erro a abrir a memória partilhada.\n");
		exit(0);
	}
	/* Definir tamanho da estrutura a passar para a memória partilhada */
	int sizeR = sizeof(readers);
	int sizeS = 100;
	/* Definir o tamanho da memória partilhada */
	if(ftruncate (fdR, sizeR) == -1 || ftruncate(fdS, sizeS)){
		perror("Erro a definir o tamanho da memória partilhada.\n");
		exit(0);
	}
	/* Mapear memória partilhada */
	readers* sharedData = (readers*) mmap(NULL, sizeR, PROT_READ|PROT_WRITE, MAP_SHARED, fdR, 0);
	char* string = (char*) mmap(NULL, sizeS, PROT_READ|PROT_WRITE, MAP_SHARED, fdS, 0);
	/* Caso dê erro a mapear a memória partilhada */
	if(sharedData == NULL || string == NULL){
		perror("Erro a mapear a memória partilhada.\n");
		exit(0);
	}
	
	/* Criar os semáforos */
	sem_t *semWriter, *semNumReaders;
	if ((semWriter = sem_open("semWriter", O_EXCL, 0644, 1)) == SEM_FAILED || (semNumReaders = sem_open("semNumReaders", O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	//--------------------------------------------------------------------------------------------------
	
	/* Esperar que outros readers atualizem o nr de readers */
	sem_wait(semNumReaders);
	/* Incrementar o numero de readers */
	sharedData->nr_readers = sharedData->nr_readers + 1;
	/* Bloquear a escrita da string logo no primeiro reader aberto */
	if(sharedData->nr_readers == 1){
		printf("A espera que os writers terminem.\n");
		sem_wait(semWriter);
	}
	/* Permitir que outros readers atualizem o nr de readers */
	sem_post(semNumReaders);
	
	/* Imprimir o numero de readers e a string */
	printf("String: %s\nReaders: %d\n", string, sharedData->nr_readers);
	sleep(10); //Apenas para testar com varios users e writers
	
	/* Esperar que outros readers atualizem o nr de readers */
	sem_wait(semNumReaders);
	/* Decrementar o numero de readers */
	sharedData->nr_readers = sharedData->nr_readers - 1;
	/* Permitir escrita da string se este for o ultimo reader a ser fechado */
	if(sharedData->nr_readers == 0)
		sem_post(semWriter);
	
	/* Permitir que outros readers atualizem o nr de readers */
	sem_post(semNumReaders);
	
	//--------------------------------------------------------------------------------------------------
	
	/* Caso dê erro a desconectar */
	if(munmap(sharedData, sizeR) == -1 || munmap(string, sizeS) == -1) {
		perror("Erro a deconectar da memoria partilhada.\n");
		exit(0);
	}
	/* Fecha a memória partilhada */
	if(close(fdR) == -1 || close(fdS) == -1) {
		perror("Erro a fechar a memoria partilhada.\n");
		exit(0);
	}
	
	return 0;
}