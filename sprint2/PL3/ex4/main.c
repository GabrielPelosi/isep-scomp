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
#define MAX_SIZE 100

//Estrutura que conterá o array de caracteres
typedef struct{
    char arr[MAX_SIZE];
}shared_data_type;

//Método de geração aleatória de caracateres entre A e Z (que em ASCII 65 e 90, respetivamente)
void getArray(char array[MAX_SIZE]){
    int i;
    printf("Array escrito: \n");
    for(i=0;i<MAX_SIZE;i++){
        //é utilizado o 91 de forma a incluir o Z
		array[i] = (rand()%(91-65))+65;
        printf("%c ", array[i]);
	} 
    printf("\n");
}


//writer do array usando o método externo getArray
int writer(){
    char array_escrito[MAX_SIZE];
    getArray(array_escrito);

    shared_data_type *shared_data;
    int i, fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/ex4_100_caract", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, strlen(array_escrito)+1,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	for(i=0;i<MAX_SIZE;i++){
        shared_data->arr[i]=array_escrito[i];
    }
    return 0;
}

//leitor do array escrito no writer
int reader(){
	shared_data_type *shared_data;
    int i, fd, data_size = sizeof(shared_data_type);
    float soma = 0.0;
	fd = shm_open("/ex4_100_caract", O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    printf("Array Lido:\n");
    for(i=0;i<MAX_SIZE;i++){
		printf("%c ", shared_data->arr[i]);
    }
    printf("\n");
    printf("\n");
    
    for(i=0;i<MAX_SIZE;i++){
		//cast para int de forma a ter o valor ASCII dos caracteres
        soma += (int)(shared_data->arr[i]);
    }
    
    float media;
    media = soma/MAX_SIZE;
    printf("Média do ASCII: %.2f\n", media);
    return 0;
}

int main(void){
    
    writer();
    reader();
    
    return 0;

}