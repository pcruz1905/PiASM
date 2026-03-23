# Makefile for compiling main.c and asm.s with debug information

# Compiler and assembler flags
CC = gcc
AS = gcc
CFLAGS = -g -c
LDFLAGS = -g -z nostackexec

# Target executable
TARGET = main

# Object files
OBJS = main.o asm.o

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

# Compile main.c into an object file
main.o: main.c asm.h
	$(CC) $(CFLAGS) main.c -o main.o

# Assemble asm.s into an object file
asm.o: asm.s
	$(AS) $(CFLAGS) asm.s -o asm.o

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program in debug mode using GDB
debug: $(TARGET)
	gdb ./$(TARGET)

# Run the program normally
run: $(TARGET)
	./$(TARGET)
