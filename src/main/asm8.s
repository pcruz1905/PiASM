.global sort_array              # Torna a função 'sort_array' acessível globalmente
.text                           # Inicia a seção de código (onde o código executável é armazenado)
sort_array:                     # Define o ponto de entrada da função 'sort_array'

    # %rdi = ponteiro para o vetor (int* vec)
    # %esi = comprimento do vetor (int length)
    # %rdx = ordem (char order)

    cmp $0, %esi                 # Compara o comprimento do vetor (%esi) com 0
    jle error                    # Se o comprimento for menor ou igual a 0, vai para 'error' (retorna 0)

    movq %rdi, %r8               # Salva o ponteiro do vetor (%rdi) em %r8
    movq $0, %r9                 # Inicializa o índice de loop (%r9) em 0 (começa do primeiro elemento)

for_loop:                        # Início do loop para percorrer os elementos do vetor
    addq $1, %r9                 # Incrementa o índice (%r9) em 1 (avança para o próximo elemento)
    cmp %r9, %rsi                # Compara o índice com o comprimento do vetor (%rsi)
    je end                       # Se o índice for igual ao comprimento, termina a execução
    movl (%rdi, %r9, 4), %r10d   # Carrega o valor do vetor na posição %r9 em %r10d
    movq %r9, %r11               # Salva o índice (%r9) em %r11 para usá-lo no loop interno
    cmp $1, %rdx                 # Compara a ordem (%rdx) com 1 (1 para crescente, 0 para decrescente)
    jl while_loop_descending      # Se a ordem for menor que 1 (decrescente), vai para 'while_loop_descending'

while_loop_ascending:            # Loop para ordenar de forma crescente
    subq $1, %r11                # Subtrai 1 de %r11 (movimenta para a esquerda do vetor)
    cmp $0, %r11                 # Verifica se %r11 é menor que 0 (fim do vetor)
    jl insert                    # Se for menor que 0, insere o valor na posição correta
    movl (%rdi, %r11, 4), %r12d   # Carrega o valor do vetor na posição %r11 em %r12d
    cmp %r10d, %r12d             # Compara o valor atual (%r10d) com o valor à esquerda (%r12d)
    jle insert                   # Se o valor atual for menor ou igual, insere na posição
    movl %r12d, 4(%rdi, %r11, 4)  # Caso contrário, move o valor à esquerda para a posição atual
    jmp while_loop_ascending      # Volta para continuar o loop

while_loop_descending:           # Loop para ordenar de forma decrescente
    subq $1, %r11                # Subtrai 1 de %r11 (movimenta para a esquerda do vetor)
    cmp $0, %r11                 # Verifica se %r11 é menor que 0 (fim do vetor)
    jl insert                    # Se for menor que 0, insere o valor na posição correta
    movl (%rdi, %r11, 4), %r12d   # Carrega o valor do vetor na posição %r11 em %r12d
    cmp %r12d, %r10d             # Compara o valor à esquerda (%r12d) com o valor atual (%r10d)
    jle insert                   # Se o valor à esquerda for menor ou igual, insere na posição
    movl %r12d, 4(%rdi, %r11, 4)  # Caso contrário, move o valor à esquerda para a posição atual
    jmp while_loop_descending     # Volta para continuar o loop

insert:                          # Insere o valor na posição correta após o loop interno
    movl %r10d, 4(%rdi, %r11, 4)  # Coloca o valor de %r10d na posição do vetor %r11
    jmp for_loop                  # Volta para o loop principal para verificar o próximo elemento

end:                             # Fim da função de ordenação
    movq $1, %rax                 # Coloca 1 em %rax (indica sucesso)
    ret                            # Retorna da função

error:                           # Caso o comprimento do vetor seja menor ou igual a zero
    movq $0, %rax                 # Coloca 0 em %rax (indica erro)
    ret                            # Retorna da função
