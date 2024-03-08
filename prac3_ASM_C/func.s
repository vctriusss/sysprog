.section .text
.globl crossOut

# *matrix - rdi
# *newMatrix - rsi
# cols - rdx
# total - rcx
# exclude_row - r8
# exclude_col - r9

crossOut:
    pushq %rcx
    pushq %rdx
    xorq %rax, %rax
    xorq %rbx, %rbx # matrix ctr
    xorq %rcx, %rcx # newMatrix ctr
    xorq %rdx, %rdx

    jmp loop

loop:
    cmpq 0x8(%rsp), %rbx
    je end

    movq %rbx, %rdx
    shrq $32, %rdx
    movq %rbx, %rax

    divq (%rsp)

    incq %rbx

    cmpl %r8d, %eax
    je loop
    cmpl %r9d, %edx
    je loop

    movq -0x4(%rdi, %rbx, 4), %rdx
    movq %rdx, (%rsi, %rcx, 4)

    incq %rcx
    jmp loop

end:
    popq %rdx
    popq %rcx
    ret
