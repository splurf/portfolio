# count the minimum, maximum, and mode lengths from stdin

.data
    array:      .space      512
    buf:        .space      512
    len:                    512
    str:        .asciiz     "-character words: "
    new_line:   .asciiz     "\n"

    min:        .asciiz     "word length minimum: "
    max:        .asciiz     "word length maximum: "
    mode:       .asciiz     "word length mode: "


.text

main:
    li      $s0     0               # string buf
    li      $s1     0               # current character
    li      $s2     0               # current length

    li      $s3     99              # min length
    li      $s4     0               # max length

    li      $s5     1               # index counter
    li      $s6     0               # mode


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

    lb      $s7     array($s2)      # load the number of encounters into $t3
    addi    $s7     $s7     1       # increment
    sb      $s7     array($s2)      # put it back

    bgt     $s7     $s6     set_mode
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


set_mode:
    move    $s6     $s7
    li      $s2     0               # reset the word length counter
    j       next                    # jump to next


set_max:
    move    $s4     $s2
    li      $s2     0               # reset the word length counter
    j       next                    # jump to next


set_min_helper:
    move    $s3     $s2
    li      $s2     0               # reset the word length counter
    j       exit                    # jump to next


set_min:
    bgtz    $s2     set_min_helper
    addi    $s5     $s5     1


exit:
    lb      $s2     array($s5)
    blt     $s2     $s3     set_min
    addi    $s5     $s5     1


finish:
    ble     $s5     $s4     exit


print:
    la      $a0     min             # copy str to $a0
    li      $v0     4               # print_string service
    syscall

    move    $a0     $s3             # print minimum
    li      $v0     1
    syscall

    la      $a0     new_line        # print new line
    li      $v0     4
    syscall

    la      $a0     max             # copy str to $a0
    li      $v0     4               # print_string service
    syscall

    move    $a0     $s4             # print maximum
    li      $v0     1
    syscall

    la      $a0     new_line        # print new line
    li      $v0     4
    syscall

    la      $a0     mode            # copy str to $a0
    li      $v0     4               # print_string service
    syscall

    move    $a0     $s5             # print mode
    li      $v0     1
    syscall