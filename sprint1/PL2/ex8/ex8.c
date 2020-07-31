#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct{
	char win[10];
	int num;
}jogo;

int main() {
	pid_t p;
	
	jogo a;
	strcpy(a.win,"Win!!!");
	
	int fd[2];
	int i, x, y[10], z[10];
	
	if(pipe(fd)==-1){
		perror("Erro no pipe!");
		return -1;
	}
	
	for(i=0; i<10; i++){
		p = fork();
		if(p==-1){
			printf("Erro no fork!\n");
			return -1;
		}
		if(p==0){
			close(fd[1]);
			if(read(fd[0], &a,sizeof(jogo))!=0){
				printf("%s Round %d\n",a.win, a.num);
				exit(i+1);
			}
			close(fd[0]);
		}
	}
	
	for(i=0; i<10; i++){
		sleep(0);
		a.num = i+1;
		close(fd[0]);
		write(fd[1], &a, sizeof(jogo));
		y[i] = wait(&x);
		z[i] = WEXITSTATUS(x);
	}
	close(fd[1]);
	
	for(i=0; i<10; i++){
		printf("Round: %d	PID:%i\n", z[i], y[i]);
	}
}
