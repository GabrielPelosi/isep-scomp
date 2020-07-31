ex7 : main.o
	gcc main.o -o ex7


main.o : ex7.c
	gcc -g -Wall -c ex7.c -o main.o

clean:
	rm -f *.o ex7

run : ex7
	./ex7
