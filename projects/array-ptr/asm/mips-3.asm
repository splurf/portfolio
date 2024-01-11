# 3.1
main_1: # `nop` do nothing in 3 different ways
    add $zero, $zero, $zero # method 1
    nop                     # method 2
    sub $zero, $zero, 0     # method 3



# 3.2
main_2: # load 32-bit immediate value into s0
    lui, $s0, 0x0004
    ori, $s0, $s0, 0x0008



# 3.3
main_3: # (int) (253 / 8) = 31 (without remainder)
    li $t0 0   # index counter variable 
    li $s1 253 # test-variable A
    li $s2 8   # test-variable B
    li $s0 0   # test-result   C

loop:
    add $s0 $s2 $s0           # add s2 to s0
    bgt $s0 $s1 print_result  # if greater, then break => prevents infinite recursion
    addi $t0 $t0 1            # add 1 to index counter
    bne $s1 $s0 loop          # if equal, then break => no remainder

print_result:
    move $a0 $t0
    li $v0 1
    syscall # this should print 31