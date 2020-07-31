#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(){

	const int ESCRITA = 1;
	const int LEITURA = 0;
	int fd[2];
	pid_t pid, *pid_p;

	if(pipe(fd) == -1){
		printf("Pipe failed");
		return 1;
	}

	if (fork() != 0) {
		pid = getpid();
		printf("PID escrito pelo pai = %d\n", pid);
		pid_p = &pid;

		/* fecha a extremidade não usada */
		close(fd[LEITURA]);
		/* escreve no pipe */
		write(fd[ESCRITA], pid_p, sizeof(pid));
		/* fecha a extremidade não usada */
		close(fd[ESCRITA]);
	}else{
		pid_p = &pid;
		/* fecha a extremidade não usada */
		close(fd[ESCRITA]);
		/* lê dados do pipe */
		read(fd[LEITURA], pid_p, sizeof(pid));
		printf("PID lido pelo filho = %d\n", pid);
		/* fecha a extremidade de leitura */
		close(fd[LEITURA]);
	}

}
