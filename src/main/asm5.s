.section	.text
	.globl	dequeue_value

dequeue_value:
	pushq	%rbp                
	movq	%rsp, %rbp          
	
	movq	%rdi, -8(%rbp)      
	movl	%esi, -12(%rbp)     
	movq	%rdx, -24(%rbp)     
	movq	%rcx, -32(%rbp)     
	movq	%r8, -40(%rbp)      


	movq	-24(%rbp), %rax     
	movl	(%rax), %edx        
	movq	-32(%rbp), %rax     
	movl	(%rax), %eax        
	cmpl	%eax, %edx          
	jne	fila_nao_vazia       
	movl	$0, %eax            # Retorna 0 (empty)
	jmp	retorno

fila_nao_vazia:
	
	movq	-24(%rbp), %rax    
	movl	(%rax), %eax        
	cltq                    #  eax (32 bits) -> rax (64 bits)
	leaq	0(,%rax,4), %rdx    # * 4 (int)
	movq	-8(%rbp), %rax      
	addq	%rdx, %rax          
	movl	(%rax), %edx       
	movq	-40(%rbp), %rax     
	movl	%edx, (%rax)        

	
	movq	-24(%rbp), %rax     
	movl	(%rax), %eax        
	addl	$1, %eax            
	cltd                    # edx:eax
	idivl	-12(%rbp)           #  tail % length
	movq	-24(%rbp), %rax     
	movl	%edx, (%rax)        

	movl	$1, %eax            # Retorna 1

retorno:
	popq	%rbp                
	ret                     
