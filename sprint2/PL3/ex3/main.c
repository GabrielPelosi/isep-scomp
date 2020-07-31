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
#define MAX_SIZE 10

typedef struct{
    int arr[MAX_SIZE];
}shared_data_type;

void getArray(int array[MAX_SIZE]){
    int i;
    printf("Array escrito: \n");
    for(i=0;i<MAX_SIZE;i++){
		array[i]=rand()%20 + 1;
        printf("%d, ", array[i]);
	} 
    printf("\n");
}


//writer
int writer(){
    int array_escrito[MAX_SIZE];
    getArray(array_escrito);

    shared_data_type *shared_data;
    int i, fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/ex3_10_inteiros", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	for(i=0;i<MAX_SIZE;i++){
        shared_data->arr[i]=array_escrito[i];
    }
    return 0;
}

//leitor
int reader(){
	shared_data_type *shared_data;
    int i, fd, data_size = sizeof(shared_data_type);
    float soma = 0.0;
	fd = shm_open("/ex3_10_inteiros", O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    printf("Array Lido:\n");
    for(i=0;i<MAX_SIZE;i++){
		printf("%d, ", shared_data->arr[i]);
    }
    printf("\n");
    printf("\n");
    
    for(i=0;i<MAX_SIZE;i++){
		soma += shared_data->arr[i];
    }
    
    float media;
    media = soma/MAX_SIZE;
    printf("Média: %.2f\n", media);
    return 0;
}

int main(void){

    writer();
    reader();
    
    return 0;

}