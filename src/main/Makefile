# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
C_SRCS = main.c machine_manager.c
ASM_SRCS = asm.s asm1.s asm2.s asm3.s asm4.s asm5.s asm6.s asm7.s asm8.s asm9.s

# Object files (replace .c and .s with .o)
C_OBJS = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.s=.o)

# Executable name
TARGET = output_program

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(C_OBJS) $(ASM_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile C files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to assemble assembly files into object files
%.o: %.s
	$(CC) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(C_OBJS) $(ASM_OBJS) $(TARGET)
