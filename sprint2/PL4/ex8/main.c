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
#define MAX_SIZE 3


int main(void){
    int i, semS=5, semC=4;
    sem_t *semp, *semf;
    pid_t pid;
    char p1='S', p2='C';

    //Semáforo para os S´s
    if ((semp = sem_open("ex8_S", O_CREAT , 0644, semS))== SEM_FAILED) {
        perror("Error in sem_open() or sem already exists");
        exit(1);
    }

    //Semáforo para os C´s
    if ((semf = sem_open("ex8_C", O_CREAT , 0644, semC))== SEM_FAILED) {
        perror("Error in sem_open() or sem already exists");
        exit(1);
    }

    for(i=0;i<MAX_SIZE;i++){
    pid = fork (); 
    if (pid > 0){ //Pai
        sem_wait(semp);
        printf("%c\n", p1);
        sem_post(semf);
    }else{ //Filho
        sem_wait(semf);
        printf("%c\n", p2);
        sem_post(semp);
    }
    }
    unlink("ex8_S");
    unlink("ex8_C");

    return 0;

}