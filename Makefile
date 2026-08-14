all: shell bin/ls

shell: main.o commands.o
	gcc -o shell main.o commands.o
	
main.o: main.c
	gcc -c main.c

commands.o: commands.c commands.h
	gcc -c commands.c
	
bin/ls: bin/ls.o
	gcc -o bin/ls bin/ls.o

bin/ls.o: bin/ls.c
	gcc -c bin/ls.c -o bin/ls.o
	
clean:
	rm -f *.o shell bin/ls bin/ls.o
	