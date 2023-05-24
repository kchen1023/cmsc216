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
