main: main.o asm.o usac09.o
	gcc main.o asm.o usac09.o -z noexecstack -o main

main.o: main.c asm.h
	gcc -g -Wall -Wextra -fanalyzer -c main.c -o main.o

asm.o: asm.s
	as asm.s -o asm.o

usac09.o: ../USAC09/asm.s
	as ../USAC09/asm.s -o usac09.o

run: main
	./main

clean:
	rm *.o main
