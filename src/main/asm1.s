.section .text
    .global get_number_binary

get_number_binary:
    cmp $0, %edi               # Check if n < 0
    jl .out_of_range           # If n < 0, return 0
    cmp $31, %edi              # Check if n > 31
    jg .out_of_range           # If n > 31, return 0

    mov $4, %rcx               # Initialize counter for 5 bits (from 4 to 0)

.loop:
    mov %edi, %eax             # Copy n to %eax
    shr %cl, %eax              # Shift n right by current bit position
    and $1, %eax               # Isolate least significant bit (0 or 1)
    mov %al, (%rsi, %rcx, 1)   # Store the binary digit (0 or 1) in bits[4 - rcx]

    sub $1, %rcx               # Move to the next bit
    cmp $-1, %rcx              # Check if all bits are processed
    jg .loop                   # Repeat if rcx >= 0

    mov $1, %eax               # Return 1 on success
    ret

.out_of_range:
    mov $0, %eax               # Return 0 if n is out of range
    ret
