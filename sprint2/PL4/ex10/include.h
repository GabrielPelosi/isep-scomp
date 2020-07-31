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

#define NUMBER_OF_RECORDS 100

#define SHM_FILE_NAME "/ex10_shm_file_name"

#define SEMAFORO_FILE_NAME "semaforo_ex_10"

typedef struct {
    char name[255];
    char address[255];
    int number;
} Record;

typedef struct {
    Record records[NUMBER_OF_RECORDS];
} SharedDataStruct;

// Semaforos
//======================================================================

void __semWAIT(sem_t *semaforo) {
    if (sem_wait(semaforo) == -1) {
        perror("Erro sem_wait()!");
        exit(EXIT_FAILURE);
    }
}

void __semPOST(sem_t *semaforo) {
    if (sem_post(semaforo) == -1) {
        perror("Erro sem_post()!");
        exit(EXIT_FAILURE);
    }
}

// Cria o semaforo
void __semaforo__CREATE_AND_OPEN(sem_t *semaforo, char* fileName, int initialValue) {
    if ((semaforo = sem_open(fileName, O_CREAT | O_EXCL, 0644, initialValue)) == SEM_FAILED) {
        perror("Erro sem_open()!\n");
        exit(EXIT_FAILURE);
    }
}

// Abre o semaforo
void __semaforo__OPEN(sem_t *semaforo, char* fileName) {
    if ((semaforo = sem_open(fileName, 0)) == SEM_FAILED) {
        perror("Erro sem_open()!\n");
        exit(EXIT_FAILURE);
    }
}

void __semaforo__CLOSE(sem_t *semaforo) {
    if (sem_close(semaforo) < 0) {
        perror("Erro sem_close()!\n");
        exit(EXIT_FAILURE);
    }
}

void __semaforo__UNLINK(char* fileName) {
    if (sem_unlink(fileName) != 0) {
        int err = errno;
        printf("Error: %s", strerror(err));
        printf("Erro shm_unlink() do semaforo!\n");
        exit(EXIT_FAILURE);
    }
}

// Memoria Partilhada
//======================================================================

// int oflag = O_RDWR | O_EXCL | O_CREAT

SharedDataStruct *__shmOPEN(char *fileName, int *fd, int oflag) {
    // Abre a memoria partilhada
    *fd = shm_open(fileName, oflag, S_IRUSR | S_IWUSR);
    if (*fd == -1) {
        perror("Erro shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Define tamanho da estrutura a passar para a memoria partilhada
    ftruncate(*fd, sizeof(SharedDataStruct));

    // Mapea memoria partilhada
    SharedDataStruct *sharedData = (SharedDataStruct *)mmap(NULL, sizeof(SharedDataStruct), PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);

    return sharedData;
}

void __shmCLOSE(SharedDataStruct *sharedData, int fd) {
    // Remove o mapeamento
    if (munmap((void *)sharedData, sizeof(SharedDataStruct)) < 0) {
        perror("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }
    
    if (close(fd) < 0) {
        perror("Erro a fechar!\n");
        exit(EXIT_FAILURE);
    }
}

void __shmDELETE(char *fileName) {
    // Remove o ficheiro do sistema
    if (shm_unlink(fileName) < 0) {
        perror("Erro shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }
}
