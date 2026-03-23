.section .text
.global get_n_element


get_n_element:
    movl (%rcx), %r8d
    movl (%rdx), %r9d


    cmpl %r8d, %r9d
    jne calc_elements

    movl $0, %eax
    ret

calc_elements:

    movl %r8d, %eax
    cmpl %r9d, %eax
    jge greater_equal

    movl %esi, %eax
    subl %r8d, %r9d
    subl %r9d, %eax
    ret

greater_equal:
    subl %r9d, %eax
    ret
    