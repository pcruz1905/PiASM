.global format_command
.text
    format_command:
        # %rdi=char* op
        # %esi=int n
        # %rdx=char* cmd
        # %r10=index tracker for cmd
        movq $0, %r10


        movq $0, %r8
        movq $0, %r13
        trim:
            movb (%rdi, %r13, 1), %r9b
            cmp $0, %r9b
            je capitalize
            cmp $' ', %r9b
            jne skip
            loop_trim:
                addq $1, %r8
                movb (%rdi, %r8, 1), %r14b
                cmp $0, %r14b
                je done
                cmp $' ', %r14b
                je loop_trim
                movb %r14b, (%rdi, %r13, 1)
                movb $' ', (%rdi, %r8, 1)
                addq $1, %r13
                movq %r13, %r8
                jmp trim

            skip:
                addq $1,%r13
                addq $1,%r8
                jmp trim

            done:
                movb $0, (%rdi, %r13, 1)
        
        capitalize:
            movq $-1, %r8
            loop_capitalize:
                addq $1, %r8
                movb (%rdi, %r8, 1), %r9b
                cmp $0, %r9b
                je validate_op
                cmp $'Z', %r9b
                jle loop_capitalize
                subb $32, %r9b
                movb %r9b, (%rdi, %r8, 1)
                jmp loop_capitalize




        validate_op:
            movq $-1, %r8
            # check if the first leter is an O
            addq $1, %r8
            movb (%rdi, %r8, 1), %r9b
            cmp $'O', %r9b
            jne error
            # check if the second is a N, if it is, add it to cmd
            addq $1, %r8
            movb (%rdi, %r8, 1), %r9b
            cmp $'N', %r9b
            je add_op_to_cmd
            # check if its a P
            cmp $'P', %r9b
            je add_op_to_cmd
            # check if its a F
            cmp $'F', %r9b
            jne error
            # check if the third letter is another F
            addq $1, %r8
            movb (%rdi, %r8, 1), %r9b
            cmp $'F', %r9b
            jne error
            

        add_op_to_cmd:
            movq $-1, %r8
            loop_add_op_to_cmd:
                addq $1, %r8
                movb (%rdi, %r8, 1), %r9b
                cmp $0, %r9b
                je conv_binary
                movb %r9b, (%rdx, %r10, 1)
                addq $1, %r10 # set index of next char to be added on cmd
                jmp loop_add_op_to_cmd            


        conv_binary:
            call add_comma
            movq $0, %r8
            movl %esi, %r8d # save value of esi
            loop_find_first_one:
                roll $1, %esi # rotates bits to the left and sends bit on carry to the lowest bit
                jc add_one
                jmp loop_find_first_one

            loop_conv_binary:
                cmp %esi, %r8d
                je end
                roll $1, %esi
                jc add_one
                movb $'0', (%rdx, %r10, 1)
                addq $1, %r10
                call add_comma
                jmp loop_conv_binary

            add_one:
                movb $'1', (%rdx, %r10, 1)
                addq $1, %r10
                call add_comma
                jmp loop_conv_binary


        add_comma:
            movb $',', (%rdx, %r10, 1)
            addq $1, %r10
            ret
            
        error:
            movq $0, %rax
            ret

        end:
            movb $0, -1(%rdx, %r10, 1) # remove last comma
            movq $1, %rax
            ret





            