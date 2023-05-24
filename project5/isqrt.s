# Kristin Chen, 118035390, kchen14

isqrt:
   # PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8

   # BODY 
   bge $a0, 2, rec            # if (j >= 2), goto rec
   move	$v0, $a0             # else return
   j	ret
   
rec:
   subu	$sp, $sp, 4          # save arg j on stack
   sw 	$a0, 4($sp)          # store curr arg
   srl	$a0, $a0, 2	         # $a0 = j >> 2
   jal  isqrt 		              # call isqrt
   # $v0 = isqrt(j >> 2)

   sll $t0, $v0, 1            # $t0 = small = isqrt(j >> 2) << 1
   addu $t1, $t0, 1           # $t1 = large = small + 1

   # if (large * large > n)
   mul $t2, $t1, $t1          # $t2 = large * large
   # > n
   lw $t3, 4($sp)             # $t2 = j (that was saved on stack)
   bgt $t2, $t3, ret_small    # if (large * large > j), return small
   move $v0, $t1              # else, return large
   j ret

ret_small:
   move $v0, $t0              # return small

ret:
   # EPILOGUE
   move	$sp, $fp
   lw	$ra, ($fp)
   lw	$fp, -4($sp)
   jr 	$ra
