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
#define MAX_SIZE 100000

typedef struct{
    int arr[MAX_SIZE];
}shared_data_type;

void getArray(int array[MAX_SIZE]){
    int i;
    for(i=0;i<MAX_SIZE;i++){
		array[i]=rand()%50;
	} 
}


//writer
int writer(){
    shared_data_type *shared_data;
    int i, fd, data_size = sizeof(shared_data_type), nums[MAX_SIZE];
    getArray(nums);
	fd = shm_open("/ex2_timer", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	for(i=0;i<MAX_SIZE;i++){
        shared_data->arr[i]=nums[i];
    }
    return 0;
}

//leitor
int reader(){
	shared_data_type *shared_data;
    int fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/ex2_timer", O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    printf("Array:\n");
    printf ("%d ", shared_data->arr[0]);
	printf ("%d ", shared_data->arr[1]);
    printf ("%d ", shared_data->arr[2]);
    printf ("%d ", shared_data->arr[3]);
    printf ("%d ", shared_data->arr[4]);
    printf ("%d ", shared_data->arr[5]);
    printf ("%d ", shared_data->arr[0]);
    printf("etc...\n");

    return 0;
}

int main(void){
    const int ESCRITA = 1;
	const int LEITURA = 0;
    int i, fd[2], nums[MAX_SIZE], numsLidos[MAX_SIZE];
	pid_t p;
    float f1, f2;


    if(pipe(fd) == -1){
		printf("Pipe failed");
		return 1;
	}

    clock_t start1,end1, start2,end2;


    start1=clock();
    //some code or function blocks whose time you want to check the time complexity

    writer();
    reader();
    

    end1=clock();
    f1=(float)(end1-start1)/CLOCKS_PER_SEC;//(gives the answer as a floating point value)
    printf("Tempo da memória partilhada: %f\n", f1);

    start2=clock();
    //some code or function blocks whose time you want to check the time complexity

    getArray(nums);

    p = fork();
    for(i=0;i<MAX_SIZE;i++){
	if (p > 0) {

		/* fecha a extremidade não usada */
		close(fd[LEITURA]);
		/* escreve no pipe */
        write(fd[ESCRITA], &nums[i], sizeof(nums));   

		close(fd[ESCRITA]);

	}else{
        i=0;
		/* fecha a extremidade não usada */
		close(fd[ESCRITA]);
		/* lê dados do pipe */
		read(fd[LEITURA], &numsLidos[i], sizeof(numsLidos));
		/* fecha a extremidade de leitura */
		close(fd[LEITURA]);
		exit(0);
	}
    }

    end2=clock();
    f2=(float)(end2-start2)/CLOCKS_PER_SEC;//(gives the answer as a floating point value)
    printf("Tempo do pipe: %f\n", f2);
    return 0;

}