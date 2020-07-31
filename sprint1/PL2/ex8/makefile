ex8 : main.o
	gcc main.o -o ex8


main.o : ex8.c
	gcc -g -Wall -c ex8.c -o main.o

clean:
	rm -f *.o ex8

run : ex8
	./ex8
