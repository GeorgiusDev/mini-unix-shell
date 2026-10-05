all: shell bin/ls bin/cat bin/grep bin/whoami bin/hostname

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
	gcc -c bin/cat.c -o bin/cat.o
	
bin/grep: bin/grep.o
	gcc -o bin/grep bin/grep.o

bin/grep.o: bin/grep.c
	gcc -c bin/grep.c -o bin/grep.o

bin/whoami: bin/whoami.o
	gcc -o bin/whoami bin/whoami.o

bin/whoami.o: bin/whoami.c
	gcc -c bin/whoami.c -o bin/whoami.o

bin/hostname: bin/hostname.o
	gcc -o bin/hostname bin/hostname.o

bin/hostname.o: bin/hostname.c
	gcc -c bin/hostname.c -o bin/hostname.o
	
clean:
	rm -f *.o shell bin/ls bin/ls.o bin/cat bin/cat.o bin/grep bin/grep.o bin/whoami bin/whoami.o bin/hostname bin/hostname.o
	