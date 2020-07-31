#include "include.h"

int main(int argc, char *agrv[]) {
    printf("Consultar todos os registos\n");
    printf("------------------------------------\n");

    // Declara o semaforo
    sem_t *semaforo;

    if ((semaforo = sem_open(SEMAFORO_FILE_NAME, 0)) == SEM_FAILED) {
        perror("Erro sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Abre a memoria partilhada
    int fd;
    SharedDataStruct *sharedData = __shmOPEN(SHM_FILE_NAME, &fd, O_RDWR /* | O_EXCL | O_CREAT*/);


    __semWAIT(semaforo);
    int found = 0;
    int i;
    for (i = 0; i < NUMBER_OF_RECORDS; i++) {
        if (sharedData->records[i].number != 0) {
            printf("Número: %d\n", sharedData->records[i].number);
            printf("Nome: %s\n", sharedData->records[i].name);
            printf("Morada: %s\n", sharedData->records[i].address);
            printf("------------------------------------\n");
            found = 1;
        }
    }

    if(found == 0) {
        printf("Registo não encontrado...\n");
    }
    __semPOST(semaforo);

    // Fecha a memoria partilhada
    __shmCLOSE(sharedData, fd);

    // Fecha o semaforo
    __semaforo__CLOSE(semaforo);

    return EXIT_SUCCESS;
}
