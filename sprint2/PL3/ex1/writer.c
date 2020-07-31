//1. Criar zona
//2. Defenir o tamanho
//3. Obter apontador para lá

	/*1. Criar ou abrir uma zona de memória partilhada
		int shm_open const char name , int oflag , mode_t mode

		--oflag
	*/	

	/*2. Defenir o tamanho
		int ftruncate int fd , off_t length ... geralmente tamanho da struct
	*/

	/*3. Retornar apontador para a zona de memória
		void mmap void addr , size_t length , int prot , int flags,int fd , off_t offset);
		mmap(null, tamanho, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0)
		nota: decide q zona do tamanho de memoria sera usado
	*/
//

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#define MAX_SIZE 100

typedef struct{
	int num;
	char nome[MAX_SIZE];
} shared_data_type;


//leitor
int main(int argc, char *argv[]){
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *student_p;
	fd = shm_open("/ex1_student", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	student_p = (shared_data_type*)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	student_p->num=1181056;
	strcpy(student_p->nome, "Rui Afonso");

	return 0;
}
