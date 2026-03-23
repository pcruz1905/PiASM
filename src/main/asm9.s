.global median
.global sort_array
.text
    median:
        # %rdi=int* vec
        # %esi=int length
        # %rdx=int* me

        
        cmp $0, %esi
        jle error

       
        movq %rdi, %r8  # vec in r8
        movl %esi, %r9d # length in r9d
        movb $1, %r10b  # Order 
        call sort_array

        
        movl %r9d, %ecx
        shr $1, %ecx    # length / 2
        test %r9d, %r9d
        jns odd_length

        # Even length
        movl (%r8, %rcx, 4), %eax
        movl -4(%r8, %rcx, 4), %ebx
        addl %ebx, %eax
        shr $1, %eax
        jmp store_median

        odd_length:
        movl (%r8, %rcx, 4), %eax

        store_median:
        movl %eax, (%rdx)
        movl $1, %eax
        ret

        error:
        movl $0, %eax
        ret
