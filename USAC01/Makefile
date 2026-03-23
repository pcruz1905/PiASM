# Makefile para compilar main.c e asm.s com informações de depuração

# Flags do compilador e do montador
CC = gcc                    # Define o compilador C como 'gcc'
AS = gcc                    # Define o montador como 'gcc' (usando o gcc também para montar)
CFLAGS = -g -c               # Flags para o compilador C: -g (gera informações de depuração) e -c (compilar sem vincular)
LDFLAGS = -g -z nostackexec  # Flags para o linker: -g (gera informações de depuração) e -z nostackexec (desabilita execução de pilha)

# Arquivo executável alvo
TARGET = main                # Nome do arquivo executável gerado

# Arquivos objeto
OBJS = main.o asm.o          # Arquivos objeto que precisam ser compilados para gerar o executável

# Alvo padrão
all: $(TARGET)               # O alvo 'all' depende de $(TARGET) (geração do executável)

# Linka os arquivos objeto para criar o executável
$(TARGET): $(OBJS)           # O alvo $(TARGET) depende dos arquivos objetos $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)  # Usa o compilador C (gcc) para gerar o executável com as flags de depuração

# Compila main.c em um arquivo objeto
main.o: main.c asm.h         # O arquivo main.o depende de main.c e asm.h (arquivo de cabeçalho)
	$(CC) $(CFLAGS) main.c -o main.o  # Compila main.c com as flags de depuração para gerar o arquivo objeto main.o

# Monta asm.s em um arquivo objeto
asm.o: asm.s                  # O arquivo asm.o depende de asm.s (arquivo de montagem)
	$(AS) $(CFLAGS) asm.s -o asm.o  # Usa o montador (gcc) para gerar o arquivo objeto asm.o a partir de asm.s

# Limpa os arquivos de compilação
clean:                        # O alvo 'clean' não depende de nada
	rm -f $(OBJS) $(TARGET)  # Remove os arquivos objeto e o executável gerado

# Executa o programa em modo de depuração com o GDB
debug: $(TARGET)              # O alvo 'debug' depende do executável $(TARGET)
	gdb ./$(TARGET)          # Chama o GDB para depurar o programa executável

# Executa o programa normalmente
run: $(TARGET)                # O alvo 'run' depende do executável $(TARGET)
	./$(TARGET)               # Executa o programa normalmente sem depuração
