### Q1 ###################################
# f = g + A [B[4] - B[3]]; ##############
##########################################
.data
    A: .word  0,  1,  2,  3,  4,  5,  6,  7,  8,  9
    B: .word 10, 11, 12, 13, 14, 15, 16, 17, 18, 19

.text


init: # instantiate variable parameters
    li $s4 50 # g (immediate)
    li $s5 0  # f (immediate)
    la $s6 A  # A (array)
    la $s7 B  # B (array)


main: # perform operations
    lw $t0 12($s7) # B[3] => 13
    lw $t1 16($s7) # B[4] => 14

    li $t2 0 # B[4] - B[3] => 1
    sub $t2 $t1 $t0

    li $t0 4 # index of `B[4] - B[3]`
    mult	$t0, $t2
    mflo	$t0
    
    add $s6 $s6 $t0 # A[index + (B[4] - B[3])] => 1
    lw $t0 ($s6)

    # f
    add $s5 $s4 $t0 # g + A[B[4] - B[3]]


print_result: # print the value of f
    move $a0 $s5
    li $v0 1
    syscall # this should print `51`