    .section .text
    .global get_number

get_number:
    xor %edx, %edx              # Clear %edx (this will store the result, start at 0)
    xor %ebx, %ebx              # Clear %ecx (index counter)

loop:
    movb (%rdi, %rbx, 1), %al   # Load the current byte from str[ecx] into %al
    test %al, %al               # Test if we hit the null terminator (end of string)
    jz end                      # If it's null terminator, we are done

    cmpb $'0', %al              # Compare the current byte with '0'
    jl invalid_char             # If it's less than '0', invalid character
    cmpb $'9', %al              # Compare the current byte with '9'
    jg invalid_char             # If it's greater than '9', invalid character

    sub $'0', %al               # Convert ASCII digit to actual integer

    movzbl %al, %ecx            # Zero-extend %al to %ecx (32-bit)
    imul $10, %edx, %edx        # Multiply the current result by 10 (shift left)
    add %ecx, %edx              # Add the current digit (from %ecx) to %edx

    inc %rbx                    # Move to the next character
    jmp loop                    # Continue the loop

invalid_char:
    movl $0, (%rsi)             # Store 0 at the location pointed to by %rsi (n)
    mov $0, %eax                # Return 0 if invalid character is found
    ret

end:
    movl %edx, (%rsi)           # Store the result (integer) in *n (address pointed to by %rsi)
    mov $1, %eax                # Return 1 (success) if valid string
    ret
