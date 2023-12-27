.data
    A: .word 1, 2, 3, 4, 5, 6, 7, 8
    B: .word 0, 0, 0, 0, 0, 0, 0, 0

.text

init:
    li $s0 2
    li $s1 5
    la $s6 A
    la $s7 B

main:
    sll $t0, $s0, 2
    add $t0, $s6, $t0
    sll $t1, $s1, 2
    add $t1, $s7, $t1
    lw $s0, 0($t0)
    addi $t2, $t0, 4
    lw $t0, 0($t2)
    add $t0, $t0, $s0
    sw $t0, 0($t1)

print_array:
    lw $a0 0($s7)
    li $v0 1
    syscall

    li $a0 ' '
    li $v0 11
    syscall
    lw $a0 4($s7)
    li $v0 1
    syscall

    li $a0 ' '
    li $v0 11
    syscall
    lw $a0 8($s7)
    li $v0 1
    syscall

    li $a0 ' '
    li $v0 11
    syscall
    lw $a0 12($s7)
    li $v0 1
    syscall
    
    li $a0 ' '
    li $v0 11
    syscall
    lw $a0 16($s7)
    li $v0 1
    syscall

        li $a0 ' '
    li $v0 11
    syscall
    lw $a0 20($s7)
    li $v0 1
    syscall

        li $a0 ' '
    li $v0 11
    syscall
    lw $a0 24($s7)
    li $v0 1
    syscall

        li $a0 ' '
    li $v0 11
    syscall
    lw $a0 28($s7)
    li $v0 1
    syscall