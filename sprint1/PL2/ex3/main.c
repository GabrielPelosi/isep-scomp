
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#define BUFFER_SIZE 80
int main(){

	const int ESCRITA = 1;
	const int LEITURA = 0;
	int fd[2], a, child_pid;
	char read_msg1[BUFFER_SIZE];
	char write_msg1[BUFFER_SIZE] = "Hello World";
	char write_msg2[BUFFER_SIZE] = "Good!";
	pid_t p;

	if(pipe(fd) == -1){
		printf("Pipe failed");
		return 1;
	}
	p = fork();
	if (p > 0) {

		/* fecha a extremidade não usada */
		close(fd[LEITURA]);
		/* escreve no pipe */
		write(fd[ESCRITA], write_msg1, strlen(write_msg1)+1);
		write(fd[ESCRITA], write_msg2, strlen(write_msg2)+1);

		close(fd[ESCRITA]);
		
		child_pid = wait(&a);
		printf("%d\n", child_pid);
		/* fecha a extremidade não usada */
	}else{
		/* fecha a extremidade não usada */
		close(fd[ESCRITA]);
		/* lê dados do pipe */
		while(read(fd[LEITURA], read_msg1, BUFFER_SIZE)!=0){
		printf("%s\n", read_msg1);
		}
		/* fecha a extremidade de leitura */
		close(fd[LEITURA]);
		exit(0);
	}

}