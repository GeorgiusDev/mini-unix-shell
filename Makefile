all: shell bin/ls bin/cat

shell: main.o commands.o shell.o
	gcc -o shell main.o commands.o shell.o
	
main.o: main.c
	gcc -c main.c

commands.o: commands.c commands.h
	gcc -c commands.c
	
shell.o: shell.c shell.h
	gcc -c shell.c
	
bin/ls: bin/ls.o
	gcc -o bin/ls bin/ls.o

bin/ls.o: bin/ls.c
	gcc -c bin/ls.c -o bin/ls.o
	
bin/cat: bin/cat.o
	gcc -o bin/cat bin/cat.o
	
bin/cat.o: bin/cat.c
	gcc .c bin/cat.c -o bin/cat.o
	
clean:
	rm -f *.o shell bin/ls bin/ls.o bin/cat.c bin/cat.o
	