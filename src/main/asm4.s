.global enque_value
.text
    # %rdi=int* buffer
    # %esi=int length
    # %rdx=int* tail
    # %rcx=int* head
    # %r8d=int value
    # %rbx=number of elements currently in the array
    enque_value:
        movl $0, %ebx # register initialization
        pushq %rcx # save rcx on stack
        getNumberOfElementsInTheArray:
            cmpl %ebx, %esi
            je removeTail
            cmpq $0, (%rcx)
            je moveValuesToTheRight
            addl $1, %ebx
            addq $4, %rcx
            jmp getNumberOfElementsInTheArray

        removeTail:
            movl $0, (%rdx) # if the buffer was full we set tail to 0
            subl $1, %ebx # update the number of elements in the buffer
            
        moveValuesToTheRight:
            movslq %ebx, %rbx
            popq %rcx # restore adress of head
            pushq %rbx # save number of elements on buffer
            
            loopMoveValuesToTheRight:
                cmp $0, %rbx
                je addValue
                movl -4(%rdi, %rbx, 4), %r10d
                movl %r10d, (%rdi, %rbx, 4)
                # subtract counter
                subq $1, %rbx
                # restore original value of buffer
                jmp loopMoveValuesToTheRight

        addValue:
            popq %rbx # restore value of elements on the buffer
            movl %r8d, (%rcx) # move value to startof the list
            addq $1, %rbx # update number of elements in the array
            movslq %esi, %rsi # sign extend esi to rsi to compare with rbx
            cmpq %rbx, %rsi
            je output_if_full
            movq $0, %rax
            ret

        output_if_full:
            movq $1, %rax
            ret
