#include "include.h"

int main(int argc, char* agrv[]) {
    printf("Inserir\n");
    printf("------------------------------------\n");

    // Declara o semaforo
    sem_t *semaforo;

    if ((semaforo = sem_open(SEMAFORO_FILE_NAME, 0)) == SEM_FAILED) {
        perror("Erro sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Abre a memoria partilhada
    int fd;
    SharedDataStruct* sharedData = __shmOPEN(SHM_FILE_NAME, &fd, O_RDWR /* | O_EXCL | O_CREAT*/);

    int number;
    char name[255];
    char address[255];

    printf("Inserir o número:\n");
    scanf("%d", &number);

    printf("Inserir o nome:\n");
    scanf("%s", name);

    printf("Inserir a morada:\n");
    scanf("%s", address);

    __semWAIT(semaforo);
    int k, exist = 0;
    for(k = 0; k < NUMBER_OF_RECORDS; k++) {
        if(sharedData->records[k].number == number) {
            printf("O registo com o número %d já existe!\n", number);
            exist = 1;
            break;
        } 
    }

	if(exist == 0){
		int i;
		for (i = 0; i < NUMBER_OF_RECORDS; i++) {
			if (sharedData->records[i].number == 0) {
				sharedData->records[i].number = number;
				strcpy(sharedData->records[i].name, name);
				strcpy(sharedData->records[i].address, address);
				break;
			}
		}
		printf("Registo adicionado com sucesso!\n");
	}
    
    __semPOST(semaforo);

    // Fecha a memoria partilhada
    __shmCLOSE(sharedData, fd);

    // Fecha o semaforo
    __semaforo__CLOSE(semaforo);

    return EXIT_SUCCESS;
}
