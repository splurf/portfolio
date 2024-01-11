# count the number of n-character words up to the length of 13 from stdin

.data
    array:      .space      512
    buf:        .space      512
    len:                    512
    str:        .asciiz     "-character words: "
    new_line:   .asciiz     "\n"


.text

main:
    li      $s0     0               # string buf
    li      $s1     0               # current character
    li      $s2     0               # current length

    li      $s3     99              # min length
    li      $s4     0               # max length

    li      $s5     1               # index counter


read:
    la      $a0     buf             # buf
    la      $a1     len             # buf len
    li      $v0     8               # read_string service
    syscall

    move    $s0     $a0             # copy $a0 to $s0
    lb      $s1     ($s0)           # load the first character from $s0 into $t0
    beq     $s1     '\n'    finish  # finish if $t0 is the end of the string

done:
    beqz     $s2    next

    lb      $t3     array($s2)      # load the number of encounters into $t3
    addi    $t3     $t3     1       # increment
    sb      $t3     array($s2)      # put it back

    bgt     $s2     $s4     set_max

    li      $s2     0               # reset the word length counter
    j       next                    # jump to next


loop:
    beq     $s1     ' '     done    # increment, advance the index of $s0, repeat
    beq     $s1     '/'     done    # increment, advance the index of $s0, repeat
    beq     $s1     '\n'    read    # increment, advance the index of $s0, repeat
    
    addi    $s2     $s2     1       # increment current word length


next:
    addi    $s0     $s0     1       # increment the index of $s0
    lb      $s1     ($s0)           # load the first character from $s0 into $s2
    j       loop                    # just to loop


set_min:
    move    $s3     $s2
    li      $s2     0               # reset the word length counter
    j       next                    # jump to next


set_max:
    move    $s4     $s2
    li      $s2     0               # reset the word length counter
    j       next                    # jump to next


exit:
    lb      $t0     array($s5)

    move    $a0     $s5
    li      $v0     1
    syscall  

    la      $a0     str
    li      $v0     4
    syscall

    move    $a0     $t0
    li      $v0     1
    syscall  

    la      $a0     new_line
    li      $v0     4
    syscall

    addi    $s5     $s5     1


finish:
    ble     $s5     $s4     exit