CC = gcc
CFLAGS = -g -Wall -Wextra -fanalyzer

EXEC = main.out

all: $(EXEC)

$(EXEC): main.o asm.o
	$(CC) main.o asm.o -z noexecstack -o $(EXEC)

main.o: main.c asm.h
	$(CC) $(CFLAGS) -c main.c -o main.o

asm.o: asm.s
	$(CC) $(CFLAGS) -c asm.s -o asm.o

clean:
	rm -f *.o $(EXEC)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
