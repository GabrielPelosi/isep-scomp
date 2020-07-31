#include "include.h"

int menu() {
    int option;
    printf("1. Inserir\n");
    printf("2. Consultar um registo\n");
    printf("3. Consultar todos os registos\n");
    printf("0. Sair\n");
    scanf("%d", &option);
    return option;
}

void runProgram(char *program) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(program, program, (char *)NULL);
        exit(EXIT_FAILURE);
    }
    while (wait(NULL) > 0) {
        // Espera por todos os filhos
    };
}

int main(int argc, char *agrv[]) {

    // Declara o semaforo
    sem_t *semaforo;

    // Cria e abre semaforo
    if ((semaforo = sem_open(SEMAFORO_FILE_NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Abre memoria partilhada
    int fd;
    SharedDataStruct *sharedData = __shmOPEN(SHM_FILE_NAME, &fd, O_RDWR | O_CREAT);

    // Reinicia os registos
    __semWAIT(semaforo);
    int i;
    for(i = 0; i < NUMBER_OF_RECORDS; i++) {
        sharedData->records[i].number = 0;
    }
    __semPOST(semaforo);

    int option;

    do {
        option = menu();

        switch (option) {
            case 1:
                runProgram("./insert.o");
                break;
            case 2:
                runProgram("./consult.o");
                break;
            case 3:
                runProgram("./consult_all.o");
                break;
            case 0:
                return EXIT_SUCCESS;
            default:
                option = menu();
        }
    } while(option != 0);

    __shmCLOSE(sharedData, fd);
    __shmDELETE(SHM_FILE_NAME);
    __semaforo__CLOSE(semaforo);
    __semaforo__UNLINK(SEMAFORO_FILE_NAME);

    return EXIT_SUCCESS;
}
