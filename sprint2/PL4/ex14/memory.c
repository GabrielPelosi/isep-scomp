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
	/* Apagar memoria partilhada anterior */
	sem_unlink("semWriter");
	sem_unlink("semNumReaders");
	shm_unlink("/shmEx14_Strings");
	shm_unlink("/shmEx14_Readers");
	printf("Memorias partilhadas anteriores eliminadas.\n");
	/* Abrir memória partilhada */
	int fdS = shm_open("/shmEx14_Strings",  O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	int fdR = shm_open("/shmEx14_Readers",  O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	/* Caso dê erro a abrir a memória partilhada */
	if (fdS == -1 || fdR == -1){
		perror("Erro a abrir a memória partilhada.\n");
		exit(0);
	}
	/* Definir tamanho da estrutura a passar para a memória partilhada */
	int sizeR = sizeof(readers);
	int sizeS = 100;
	/* Definir o tamanho da memória partilhada */
	if(ftruncate (fdR, sizeR) == -1 || ftruncate (fdS, sizeS) == -1){
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
	if ((semWriter = sem_open("semWriter", O_CREAT, 0644, 1)) == SEM_FAILED || (semNumReaders = sem_open("semNumReaders", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	/* Caso dê erro a desconectar */
	if(munmap(sharedData, sizeR) == -1 || munmap(string, sizeS) == -1) {
		perror("Erro a deconectar da memoria partilhada.\n");
		exit(0);
	}
	/* Fecha a memória partilhada */
	if(close(fdS) == -1 || close(fdR) == -1) {
		perror("Erro a fechar a memoria partilhada.\n");
		exit(0);
	}
	printf("Memorias Partilhadas restauradas com sucesso.\n");
	
	return 0;
}