#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 256 
int main(){

	const int ESCRITA = 1;
	const int LEITURA = 0;
	int up[2], down[2], i, c=0;
	char ch;
	pid_t p;

	char str[MAX_SIZE];
	char ltr[MAX_SIZE];
	char finaltr[MAX_SIZE];

	if(pipe(up) == -1){
		printf("Pipe up failed");
		return 1;
	}
	if(pipe(down) == -1){
		printf("Pipe down failed");
		return 1;
	}

	p = fork();

	if(p == 0){
		close(up[LEITURA]);
   		printf("Enter string: ");
   		scanf("%s", str);
		write(up[ESCRITA], str, strlen(str)+1);
		close(up[ESCRITA]);
	}else{
		close(up[ESCRITA]);
		/* lê dados do pipe */
		read(up[LEITURA], ltr, MAX_SIZE);
		printf("Original:");
		for(i=0; i<strlen(str);i++){
			printf("%c", ltr[i]);
		}
		printf("\n");
		while (ltr[c] != '\0') {
      		ch = ltr[c];
      		if (ch >= 'A' && ch <= 'Z'){
         		ltr[c] = ltr[c] + 32; 
			}	  
			if (ch >= 'a' && ch <= 'z'){
         		ltr[c] = ltr[c] - 32;   
			}
			c++; 

   		}	
		printf("Case trocada no pai:");
		for(i=0; i<strlen(ltr);i++){
			printf("%c", ltr[i]);
		}
		printf("\n");
		/* fecha a extremidade de leitura */
		close(up[LEITURA]);
	}

	if(p!= 0){
		/* fecha a extremidade não usada */
		close(down[LEITURA]);
		/* escreve no pipe */
		write(down[ESCRITA], ltr, strlen(ltr)+1);
		/* fecha a extremidade não usada */
		close(down[ESCRITA]);
	}else{
		/* fecha a extremidade não usada */
		close(down[ESCRITA]);
		/* lê dados do pipe */
		read(down[LEITURA], finaltr, MAX_SIZE);
		printf("Case trocada no filho:");
		for(i=0; i<strlen(finaltr);i++){
			printf("%c", finaltr[i]);
		}
		printf("\n");
		/* fecha a extremidade de leitura */
		close(down[LEITURA]);
	}
}
