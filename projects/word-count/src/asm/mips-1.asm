# count the number of words from stdin

.data
    buf:    .space      512
    len:                512
    str:    .asciiz     "number of words: "

.text

# $s0 string buf
# $s1 word count
# $s2 current char

main:
    li      $s0     0               # initiate string buf
    li      $s1     0               # initiate word counter
    li      $s2     0               # initiate current char

read:
    la      $a0     buf             # buf
    la      $a1     len             # buf len
    li      $v0     8               # read_string service
    syscall

    move    $s0     $a0             # copy $a0 to $s0
    lb      $s2     ($s0)           # load the first character from $s0 into $t0
    beq     $s2     '\n'    exit    # exit if $t0 is the end of the string

incr:
    addi    $s1     $s1     1       # increment the word counter
    j       next                    # just to next

loop:
    beq     $s2     ' '     incr    # increment, advance the index of $s0, repeat
    beq     $s2     '/'     incr    # increment, advance the index of $s0, repeat
    beq     $s2     '\n'    read    # increment, advance the index of $s0, repeat

next:
    addi    $s0     $s0     1       # increment the index of $s0
    lb      $s2     ($s0)           # load the first character from $s0 into $t0
    j       loop                    # just to loop

exit:
    la      $a0     str             # copy str to $a0
    li      $v0     4               # print_string service
    syscall

    move    $a0     $s1             # copy $s1 to $a0
    li      $v0     1               # print_int service
    syscall