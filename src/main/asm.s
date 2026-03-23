.section .text                    # Define a seção de código (onde o código executável fica armazenado)
.globl extract_data               # Torna a função 'extract_data' global (acessível fora deste módulo)
extract_data:                     # Define o ponto de entrada da função 'extract_data'
    pushq   %rbp                  # Salva o valor atual do registrador de base na pilha (para preservar o quadro de pilha anterior)
    movq    %rsp, %rbp            # Define o valor do ponteiro de pilha (%rsp) no registrador de base (%rbp), criando o novo quadro de pilha

    movq    %rdi, -40(%rbp)       # Armazena o ponteiro para 'str' (primeiro argumento) em -40(%rbp)
    movq    %rsi, -48(%rbp)       # Armazena o ponteiro para 'token' (segundo argumento) em -48(%rbp)
    movq    %rdx, -56(%rbp)       # Armazena o ponteiro para 'unit' (terceiro argumento) em -56(%rbp)
    movq    %rcx, -64(%rbp)       # Armazena o ponteiro para 'value' (quarto argumento) em -64(%rbp)

    movq    -40(%rbp), %rax       # Carrega o ponteiro 'str' da pilha em %rax
    movq    %rax, -24(%rbp)       # Armazena o ponteiro 'str' em -24(%rbp) para uso posterior
    jmp     inicio_busca          # Salta para a etiqueta 'inicio_busca', que inicia a busca pelo token

inicio_verificacao_token:
    movq    -48(%rbp), %rax       # Carrega o ponteiro para o token da pilha em %rax
    movq    %rax, -16(%rbp)       # Salva o ponteiro para o token temporariamente em -16(%rbp)
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string ('ptr') da pilha em %rax
    movq    %rax, -8(%rbp)        # Salva temporariamente o ponteiro 'ptr' em -8(%rbp)
    jmp     verificar_caracteres  # Salta para 'verificar_caracteres', onde começa a comparação dos caracteres

caracter_corresponde:
    addq    $1, -16(%rbp)         # Avança no ponteiro do token (move para o próximo caractere do token)
    addq    $1, -8(%rbp)          # Avança no ponteiro da string (move para o próximo caractere da string)

verificar_caracteres:
    movq    -16(%rbp), %rax       # Carrega o ponteiro para o token em %rax
    movzbl  (%rax), %eax          # Carrega o primeiro caractere do token em %eax
    testb   %al, %al              # Testa se o caractere do token é nulo (fim do token)
    je      token_completo        # Se for nulo, o token está completo, saltando para 'token_completo'
    movq    -8(%rbp), %rax        # Carrega o ponteiro da string ('ptr') em %rax
    movzbl  (%rax), %eax          # Carrega o primeiro caractere da string em %eax
    testb   %al, %al              # Testa se o caractere da string é nulo (fim da string)
    je      token_completo        # Se for nulo, o token está completo, saltando para 'token_completo'
    movq    -16(%rbp), %rax       # Carrega o ponteiro para o token em %rax
    movzbl  (%rax), %edx          # Carrega o primeiro caractere do token em %edx
    movq    -8(%rbp), %rax        # Carrega o ponteiro da string ('ptr') em %rax
    movzbl  (%rax), %eax          # Carrega o caractere da string em %eax
    cmpb    %al, %dl              # Compara o caractere do token com o caractere da string
    je      caracter_corresponde  # Se os caracteres forem iguais, salta para 'caracter_corresponde'

token_completo:
    movq    -16(%rbp), %rax       # Carrega o ponteiro para o token em %rax
    movzbl  (%rax), %eax          # Carrega o primeiro caractere do token em %eax
    testb   %al, %al              # Testa se o caractere é nulo (fim do token)
    jne     verificar_proximo     # Se não for nulo, continua verificando o próximo caractere
    jmp     primeiro_e            # Caso contrário, salta para 'primeiro_e'

avancar_na_string:
    addq    $1, -24(%rbp)         # Avança o ponteiro da string ('ptr') em 1

primeiro_e:
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    movzbl  (%rax), %eax          # Carrega o primeiro caractere da string em %eax
    testb   %al, %al              # Testa se o caractere é nulo (fim da string)
    je      fim_string            # Se for nulo, salta para 'fim_string'
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    movzbl  (%rax), %eax          # Carrega o primeiro caractere da string em %eax
    cmpb    $38, %al              # Compara se o caractere da string é igual a '&'
    jne     avancar_na_string     # Se não for '&', avança para o próximo caractere

fim_string:
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    leaq    1(%rax), %rdx         # Avança o ponteiro da string para o próximo caractere
    movq    %rdx, -24(%rbp)       # Atualiza o ponteiro da string na pilha
    movzbl  (%rax), %eax          # Carrega o caractere atual da string em %eax
    testb   %al, %al              # Testa se o caractere é nulo (fim da string)
    jne     achar_inicio_unit     # Se não for nulo, avança para a busca da unidade ('unit')
    movl    $0, %eax              # Retorna 0 em caso de erro (não encontrou 'unit')
    jmp     final_funcao          # Salta para 'final_funcao' para finalizar a função

achar_inicio_unit:
    addq    $5, -24(%rbp)         # Avança o ponteiro para "unit:" (5 caracteres)

copiar_unit:
    movq    -24(%rbp), %rdx       # Carrega o ponteiro da string em %rdx
    leaq    1(%rdx), %rax         # Avança o ponteiro da string para o próximo caractere
    movq    %rax, -24(%rbp)       # Atualiza o ponteiro da string na pilha
    movq    -56(%rbp), %rax       # Carrega o ponteiro para 'unit' em %rax
    leaq    1(%rax), %rcx         # Avança o ponteiro para 'unit'
    movq    %rcx, -56(%rbp)       # Atualiza o ponteiro para 'unit' na pilha
    movzbl  (%rdx), %edx          # Carrega o caractere da string em %edx
    movb    %dl, (%rax)           # Copia o caractere para 'unit'

continuar_copiando_unit:
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    movzbl  (%rax), %eax          # Carrega o caractere atual da string em %eax
    testb   %al, %al              # Testa se o caractere é nulo (fim da string)
    je      final_unit            # Se for nulo, salta para 'final_unit'
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    movzbl  (%rax), %eax          # Carrega o caractere da string em %eax
    cmpb    $38, %al              # Compara se o caractere é igual a '&'
    jne     copiar_unit           # Se não for '&', continua copiando os caracteres

final_unit:
    movq    -56(%rbp), %rax       # Carrega o ponteiro para 'unit' em %rax
    movb    $0, (%rax)            # Adiciona o terminador nulo '\0' ao final de 'unit'
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    leaq    1(%rax), %rdx         # Avança o ponteiro da string para o próximo caractere
    movq    %rdx, -24(%rbp)       # Atualiza o ponteiro da string
    movzbl  (%rax), %eax          # Carrega o caractere atual da string
    testb   %al, %al              # Testa se o caractere é nulo (fim da string)
    jne     achar_inicio_valor    # Se não for nulo, avança para a busca de 'value'
    movl    $0, %eax              # Retorna 0 em caso de erro (não encontrou 'value')
    jmp     final_funcao          # Salta para 'final_funcao' para finalizar a função

achar_inicio_valor:
    addq    $6, -24(%rbp)         # Avança o ponteiro para "value:" (6 caracteres)
    movq    -64(%rbp), %rax       # Carrega o ponteiro para 'value' em %rax
    movl    $0, (%rax)            # Inicializa 'value' com 0
    jmp     continuar_valor       # Salta para 'continuar_valor' para processar os dígitos de 'value'

processar_valor:
    movq    -64(%rbp), %rax       # Carrega o ponteiro para 'value' em %rax
    movl    (%rax), %edx          # Carrega o valor atual de 'value' em %edx
    movl    %edx, %eax            # Copia o valor de 'value' para %eax
    sall    $2, %eax              # Multiplica 'value' por 4
    addl    %edx, %eax            # Adiciona 'value * 4 + value', resultando em 'value * 5'
    addl    %eax, %eax            # Multiplica por 10 ('value * 10')
    movl    %eax, %edx            # Atualiza o valor em %edx
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    movzbl  (%rax), %eax          # Carrega o caractere da string em %eax
    movsbl  %al, %eax             # Converte o caractere para inteiro
    subl    $48, %eax             # Converte o caractere ('0' - '9') para um número (subtrai 48)
    addl    %eax, %edx            # Adiciona o valor ao total de 'value'
    movq    -64(%rbp), %rax       # Carrega o ponteiro para 'value' em %rax
    movl    %edx, (%rax)          # Atualiza 'value' com o novo valor calculado
    addq    $1, -24(%rbp)         # Avança o ponteiro da string ('ptr')

continuar_valor:
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    movzbl  (%rax), %eax          # Carrega o caractere da string em %eax
    cmpb    $47, %al              # Compara o caractere da string com o valor de '0'
    jle     final_valor           # Se for menor ou igual a '9', processa o valor
    cmpb    $57, %al              # Compara o caractere da string com o valor de '9'
    jle     processar_valor       # Se estiver entre '0' e '9', processa o valor

final_valor:
    movl    $1, %eax              # Retorna 1 indicando sucesso
    jmp     final_funcao          # Salta para finalizar a função

verificar_proximo:
    addq    $1, -24(%rbp)         # Avança o ponteiro da string ('ptr')

inicio_busca:
    movq    -24(%rbp), %rax       # Carrega o ponteiro da string em %rax
    movzbl  (%rax), %eax          # Carrega o caractere da string em %eax
    testb   %al, %al              # Testa se o caractere é nulo (fim da string)
    jne     inicio_verificacao_token # Se não for nulo, começa a verificar o token
    movl    $0, %eax              # Se fim da string sem encontrar o token, retorna 0

final_funcao:
    popq    %rbp                  # Restaura o valor do registrador de base (%rbp) da pilha
    ret                           # Retorna da função
