main: main.o 
	gcc -g main.o -o main

main.o: main.c
	gcc -g -Wall -c main.c -o main.o
	
run: main
	./main
	
clean:
	rm -f *.o ex10
