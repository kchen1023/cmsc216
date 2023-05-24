# Kristin Chen, 118035390, kchen14

reverse_prefix_sum:
   # PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8

   # BODY
   lw $t0, ($a0)                # load r (array entry)
   subu $sp, $sp, 8

   bne	$t0, -1, rec            # while entry != -1
   li	$v0, 0                  # if *arr == -1, return 0
   j	ret

# a = [ 1, 2, 3, 4, -1]
# a_result = [10, 9, 7, 4]
# r5 return 0
# r4 = [ 1, 2, 7, 4]
# r3 = [ 1, 2, 7, 4]
# r2 = [ 1, 9, 7, 4]
# r1 = [ 1, 9, 7, 4]
# r0 = [10, 9, 7, 4]

# b1 = 1 + 2 + 3 + 4 = 10               b1 = a1 + a2 + a3 + a4
# b2 = 2 + 3 + 4     = 9                b2 = a2 + a3 + a4
# b3 = 3 + 4         = 7                b3 = a3 + a4
# b4 = 4             = 4                b4 = a4
# int r
# r = reverse_prefix_sum(arr + 1) + *arr;
# *arr = r; 

rec:
    sw 	$a0, 8($sp)             # store curr arr pointer
    sw $t0, 4($sp)              # save r, $t0 = r
    addu $a0, $a0, 4            # go to next entry 
    jal  reverse_prefix_sum     # call reverse prefix sum 

    lw $a0, 8($sp)              # load a0
    lw	$t0, 4($sp)	            # load r

    addu	$v0, $v0, $t0  	    # $v0 = adds returned recursive value to curr value
    sw $v0, ($a0)               # store the value in array 

    j ret
    
ret:
    # EPILOGUE
    move	$sp, $fp
    lw	$ra, ($fp)
    lw	$fp, -4($sp)
    jr 	$ra