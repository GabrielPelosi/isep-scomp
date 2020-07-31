#define ARRAY_SIZE 10002 
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main (){
	int n; /* the number to find */
	time_t t; /* needed to init. the random number generator (RNG)*/
	int i, countF = 0, countP = 0, status; 
	pid_t p;
	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));
	
	/* initialize array with random numbers (rand(): stdlib.h) */


	/* initialize n */
	//n = rand () % 10000;
	 n = 1;
	 int numbers[6]={1,1,0,0,0,0};
	
	
	p = fork();
	if (p == 0){
		for(i = 0; i < 6/2; i++){
		if(numbers[i] == n){
			countF ++;
			printf("Sou o filho, e encontrei %d : %d\n", n, countF);
		}
		printf("Sou o filho, e não encontrei %d : %d\n", n, countF);
		}
		exit(countF);
	}else{
		for(i = 3; i < 6; i++){
		if(numbers[i] == n){
			countP ++;
			printf("Sou o Pai, e encontrei %d : %d\n", n, countP);
		}
		printf("Sou o Pai, e não encontrei %d : %d\n", n, countP);
		}
		wait(&status);
	}
	
	countP = countP + WEXITSTATUS(status);
	printf("Numero de vezes: %d\n", countP);  
	return 0;
			
}
