# ex14/Makefile
reader: reader.o
	gcc -Wall -g reader.o -o reader -lm -lrt -lpthread
	
reader.o: reader.c
	gcc -Wall -g -c reader.c -o reader.o -lm -lrt -lpthread
	
runReader: reader
	./reader
	
writer: writer.o
	gcc -Wall -g writer.o -o writer -lm -lrt -lpthread
	
writer.o: writer.c
	gcc -Wall -g -c writer.c -o writer.o -lm -lrt -lpthread
	
runWriter: writer
	./writer	

memory:  memory.o 
	gcc -Wall -g  memory.o -o memory -lrt -lpthread
	
memory.o: memory.c
	gcc -Wall -g -c memory.c -o memory.o -lrt -lpthread

runMemory: memory
	./memory
	
clean:
	rm writer.o writer reader.o reader