#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int status;
	pid_t p;
	p = fork();
	if(p == 0){
		sleep(1);
		exit(1);
	}else{
		wait(&status);
		printf("%d\n", WEXITSTATUS(status));
		p = fork();
		if(p == 0){
			sleep(2);
			exit(2);
		}else{
			wait(&status);
			printf("%d\n", WEXITSTATUS(status));
		}
	}

return 0;
}
