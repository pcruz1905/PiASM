main: main.o asm.o
	gcc main.o asm.o -z noexecstack -o main

main.o: main.c asm.h
	gcc -g -Wall -Wextra -fanalyzer -c main.c -o main.o

asm.o: asm.s
	as asm.s -o asm.o

run: main
	./main

clean: main
	 rm *.o main
