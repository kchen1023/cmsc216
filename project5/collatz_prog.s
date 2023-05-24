.data

# array terminated by 0 (which is not part of the array)
xarr:
.word 2, 4, 6, 8, 10, 0
.data

arrow: .asciiz " -> "

.text

main:
    li      $sp,        0x7ffffffc      # initialize $sp

# PROLOGUE
    subu    $sp,        $sp,        8   # expand stack by 8 bytes
    sw      $ra,        8($sp)          # push $ra (ret addr, 4 bytes)
    sw      $fp,        4($sp)          # push $fp (4 bytes)
    addu    $fp,        $sp,        8   # set $fp to saved $ra

    subu    $sp,        $sp,        12  # save s0 and s1 on stack before using them
    sw      $s0,        12($sp)         # push $s0
    sw      $s1,        8($sp)          # push $s1
    sw      $s2,        4($sp)          # push $s2

    la      $s0,        xarr            # load address to s0

main_for:
    lw      $s1,        ($s0)           # use s1 for xarr[i] value
    li      $s2,        0               # use s2 for initial depth (steps)
    beqz    $s1,        main_end        # if xarr[i] == 0, stop.

# save args on stack rightmost one first
    subu    $sp,        $sp,        8   # save args on stack
    sw      $s2,        8($sp)          # save depth
    sw      $s1,        4($sp)          # save xarr[i]

    li      $v0,        1
    move    $a0,        $s1             # print_int(xarr[i])
    syscall 

    li      $v0,        4               # print " -> "
    la      $a0,        arrow
    syscall 

    jal     collatz                     # result = collatz(xarr[i])

    move    $a0,        $v0             # print_int(result)
    li      $v0,        1
    syscall 

    li      $a0,        10              # print_char('\n')
    li      $v0,        11
    syscall 

    addu    $s0,        $s0,        4   # make s0 point to the next element

    lw      $s2,        8($sp)          # save depth
    lw      $s1,        4($sp)          # save xarr[i]
    addu    $sp,        $sp,        8   # save args on stack
    j       main_for

main_end:
    lw      $s0,        12($sp)         # push $s0
    lw      $s1,        8($sp)          # push $s1
    lw      $s2,        4($sp)          # push $s2

# EPILOGUE
    move    $sp,        $fp             # restore $sp
    lw      $ra,        ($fp)           # restore saved $ra
    lw      $fp,        -4($sp)         # restore saved $fp
    jr      $ra                         # return to kernel
# Kristin Chen, 118035390, kchen14
# collatz(n, d)     // d = 0
#  if (n != 1)
#    if (n % 2)
#      return collatz(3 * n + 1, d + 1)
#    else
#      return collatz(n / 2, d + 1)
# return d          // where d is the number of times function was ran

collatz:
    # PROLOGUE
    subu $sp, $sp, 8
    sw	$ra, 8($sp)
    sw	$fp, 4($sp)
    addu	$fp, $sp, 8

    # BODY
    # if (n = 1), goto rec
    bne $s1, 1, rec
    # else (return d)
    move $v0, $s2
    j ret

rec:
    # determine if input[i] is even or odd
    rem $t0, $s1, 2     # t0 = xarr[i] % 2
    beq $t0, 1, odd     # if t0 = odd, goto odd
    # if even, continue
    div $s1, $s1, 2     # n / 2
    add $s2, $s2, 1     # d + 1
    jal collatz
    j ret

odd:
    mul $s1, $s1, 3     # n * 3
    add $s1, $s1, 1     # n * 3 + 1
    add $s2, $s2, 1     # d + 1
    jal collatz
    j ret

ret:
    # EPILOGUE
    move	$sp, $fp
    lw	$ra, ($fp)
    lw	$fp, -4($sp)
    jr 	$ra