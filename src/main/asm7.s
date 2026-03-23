.section	.text
	.globl	move_n_to_array
move_n_to_array:
	pushq	%rbp                # Salva o valor de RBP
	movq	%rsp, %rbp           # Atualiza RBP para o topo da pilha

	movq	%rdi, -24(%rbp)      # buffer
	movl	%esi, -28(%rbp)      # length
	movq	%rdx, -40(%rbp)      # tail
	movq	%rcx, -48(%rbp)      # head
	movl	%r8d, -32(%rbp)      # n
	movq	%r9, -56(%rbp)       # array

	movq	-48(%rbp), %rax      
	movl	(%rax), %edx         
	movq	-40(%rbp), %rax      
	movl	(%rax), %eax
	cmpl	%eax, %edx			 # head == tail
	jne		.check_available     

	movl	$0, %eax             # empty -> 0
	jmp		.end_function

.check_available:
	movq	-48(%rbp), %rax
	movl	(%rax), %edx
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, %edx           
	jle		.available_wrapping   # wraparound

	movq	-48(%rbp), %rax
	movl	(%rax), %edx
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	subl	%eax, %edx           # elementos disponíveis
	movl	%edx, -8(%rbp)       # Armazena

	jmp		.continue_check

.available_wrapping:
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	movl	-28(%rbp), %edx
	subl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	(%rax), %eax
	addl	%edx, %eax
	movl	%eax, -8(%rbp)

.continue_check:
	movl	-32(%rbp), %eax      # Verifica se n <= disponíveis
	cmpl	-8(%rbp), %eax
	jle		.move_elements

	movl	$0, %eax             # Se não houver elementos suficientes, retorna 0
	jmp		.end_function

.move_elements:
	movl	$0, -4(%rbp)         # i do for

.move_loop:
	movq	-40(%rbp), %rax     
	movl	(%rax), %eax
	cltq                       # Converte para 64 bits
	leaq	0(,%rax,4), %rdx     
	movq	-24(%rbp), %rax      
	addq	%rdx, %rax
	movl	-4(%rbp), %edx       
	movslq	%edx, %rdx
	leaq	0(,%rdx,4), %rcx     
	movq	-56(%rbp), %rdx      
	addq	%rcx, %rdx
	movl	(%rax), %eax         

	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	addl	$1, %eax             
	cltd
	idivl	-28(%rbp)
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)         

	addl	$1, -4(%rbp)        
	movl	-4(%rbp), %eax       
	cmpl	-32(%rbp), %eax
	jl		.move_loop

	movl	$1, %eax             

.end_function:
	popq	%rbp                 
	ret                           
