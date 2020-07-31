#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <sys/mman.h>
#define MAX_SIZE 10
#define MAX_PATH 10000000000000
#define BUFFER_SIZE 1000

//Estrutura que conterá o array de caracteres
 struct Child1{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c1;
 struct Child2{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c2;
 struct Child3{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c3;
 struct Child4{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c4;
 struct Child5{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c5;
 struct Child6{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c6;
 struct Child7{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c7;
 struct Child8{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c8;
 struct Child9{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c9;
struct Child10{
    char filePath[MAX_PATH];
    char word[MAX_SIZE];
    int numOcurr;
}c10;

typedef struct{
    struct Child1* c1;
    struct Child2* c2;
    struct Child3* c3;
    struct Child4* c4;
    struct Child5* c5;
    struct Child6* c6;
    struct Child7* c7;
    struct Child8* c8;
    struct Child9* c9;
    struct Child10* c10;
}shared_data_type;

//writer
int writer1(){
    shared_data_type *shared_data;
    int fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/ex13", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    //Data of child 1
    strcpy(&shared_data->c1->filePath[MAX_PATH],"input.txt");
    strcpy(&shared_data->c1->word[MAX_SIZE], "Rui");
    shared_data->c1->numOcurr = 0;

    //Data of child 2
    strcpy(&shared_data->c2->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c2->word[MAX_SIZE], "ISEP");
    shared_data->c2->numOcurr = 0;

    //Data of child 3
    strcpy(&shared_data->c3->filePath[MAX_PATH],"input.txt");
    strcpy(&shared_data->c3->word[MAX_SIZE],"cadeiras");
    shared_data->c3->numOcurr = 0;

    //Data of child 4
    strcpy(&shared_data->c4->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c4->word[MAX_SIZE],"frequências");
    shared_data->c4->numOcurr = 0;

    //Data of child 5
    strcpy(&shared_data->c5->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c5->word[MAX_SIZE], "passam");
    shared_data->c5->numOcurr = 0;

    //Data of child 6
    strcpy(&shared_data->c6->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c6->word[MAX_SIZE], "chumbam");
    shared_data->c6->numOcurr = 0;

    //Data of child 7
    strcpy(&shared_data->c7->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c7->word[MAX_SIZE], "alunos");
    shared_data->c7->numOcurr = 0;

    //Data of child 8
    strcpy(&shared_data->c8->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c8->word[MAX_SIZE], "tristes");
    shared_data->c8->numOcurr = 0;

    //Data of child 9
    strcpy(&shared_data->c9->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c9->word[MAX_SIZE], "felizes");
    shared_data->c9->numOcurr = 0;

    //Data of child 10
    strcpy(&shared_data->c10->filePath[MAX_PATH], "input.txt");
    strcpy(&shared_data->c10->word[MAX_SIZE], "final");
    shared_data->c10->numOcurr = 0;
    return 0;
}

//leitor do array escrito no writer
int reader1(int i){
    FILE *filePointer;
	shared_data_type *shared_data;
    int count, fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/ex13", O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(i==1){
        filePointer = fopen(&shared_data->c1->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c1->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==2){
        filePointer = fopen(&shared_data->c2->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c2->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==3){
        filePointer = fopen(&shared_data->c3->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c3->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==4){
        filePointer = fopen(&shared_data->c4->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c4->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==5){
        filePointer = fopen(&shared_data->c5->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c5->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==6){
        filePointer = fopen(&shared_data->c6->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c6->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==7){
        filePointer = fopen(&shared_data->c7->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c7->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==8){
        filePointer = fopen(&shared_data->c8->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c8->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==9){
        filePointer = fopen(&shared_data->c9->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c9->word[MAX_SIZE]);
            writer2(i,count);
    }
    }
    if(i==10){
        filePointer = fopen(&shared_data->c10->filePath[MAX_PATH],"r");
        if (filePointer == NULL){
            printf("Unable to open file.\n");
            exit(1);
        }else{
            count = countOccurrences(filePointer, &shared_data->c10->word[MAX_SIZE]);
            writer2(i,count);
        }
    }
    return (0);
}

int writer2(int i, int count){
    shared_data_type *shared_data;
    int fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/ex13", O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if(i==1){
        shared_data->c1->numOcurr = count;
    }
    if(i==2){
        shared_data->c2->numOcurr = count;
    }
    if(i==3){
        shared_data->c3->numOcurr = count;
    }
    if(i==4){
        shared_data->c4->numOcurr = count;
    }
    if(i==5){
        shared_data->c5->numOcurr = count;
    }
    if(i==6){
        shared_data->c6->numOcurr = count;
    }
    if(i==7){
        shared_data->c7->numOcurr = count;
    }
    if(i==8){
        shared_data->c8->numOcurr = count;
    }
    if(i==9){
        shared_data->c9->numOcurr = count;
    }
    if(i==10){
        shared_data->c10->numOcurr = count;
    }
    return(0);
}

int reader2(){
	shared_data_type *shared_data;
    int fd, data_size = sizeof(shared_data_type);
	fd = shm_open("/ex13", O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	shared_data = (shared_data_type *)mmap(NULL, data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c1->word[MAX_SIZE], shared_data->c1->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c2->word[MAX_SIZE], shared_data->c2->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c3->word[MAX_SIZE], shared_data->c3->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c4->word[MAX_SIZE], shared_data->c4->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c5->word[MAX_SIZE], shared_data->c5->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c6->word[MAX_SIZE], shared_data->c6->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c7->word[MAX_SIZE], shared_data->c7->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c8->word[MAX_SIZE], shared_data->c8->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c9->word[MAX_SIZE], shared_data->c9->numOcurr);
    printf("Número de ocurrências da palavra %s: %d\n", &shared_data->c10->word[MAX_SIZE], shared_data->c10->numOcurr);
    return(0);
}

int main(void){
    pid_t p;
    p=fork();
    if(p>0){ //pai
        writer1();
        wait(NULL);
        reader2();
    }else{
        //1º filho, logo index é 1
        p=fork();
        if(p>0){
            reader1(1);
        }else{
            //2º filho, logo index é 2
            p=fork();
            if(p>0){
                reader1(2);
            }else{
                //3º filho, logo index é 3
                p=fork();
                if(p>0){
                  reader1(3);  
                }else{
                    //4º filho, logo index é 4
                    p=fork();
                    if(p>0){
                      reader1(4);  
                    }else{
                        //5º filho, logo index é 5
                        p=fork();
                        if(p>0){
                            reader1(5);
                        }else{
                            //6º filho, logo index é 6
                            p=fork();
                            if(p>0){
                                reader1(6);
                            }else{
                                //7º filho, logo index é 7
                                p=fork();
                                if(p>0){
                                    reader1(7);  
                                }else{
                                    //8º filho, logo index é 8
                                    p=fork();
                                    if(p>0){
                                        reader1(8);  
                                    }else{
                                        //9º filho, logo index é 9
                                        p=fork();
                                        if(p>0){
                                           reader1(9); 
                                        }else{
                                            //10º filho, logo index é 10
                                            reader1(10);
                                        } 
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 0;

}

int countOccurrences(FILE *filePointer, const char *word){
    char str[BUFFER_SIZE];
    char *pos;

    int index, count;
    
    count = 0;

    // Read line from file till end of file.
    while ((fgets(str, BUFFER_SIZE, filePointer)) != NULL)
    {
        index = 0;

        // Find next occurrence of word in str
        while ((pos = strstr(str + index, word)) != NULL)
        {
            // Index of word in str is
            // Memory address of pos - memory
            // address of str.
            index = (pos - str) + 1;

            count++;
        }
    }

    return count;
}