   .data
# uint arrays, each terminated by -1 (which is not part of array)
arr0:
   .word 1, 2, 3, 4, -1
arr1:
   .word 2, 3, 4, 5, -1
arr2:
   .word 5, 4, 3, 2,  -1
arr3:
   .word 1, 2, 1, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 4, -1
overflow:
   .word 1, 1, 1, 1, 2147483646, -1

   .text

# main(): ##################################################
#   process_array(arr0)
#   process_array(arr1)
#   process_array(arr2)
#   process_array(arr3)
#   process_array(arr4)
#
main:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   la   $a0, arr0
   jal  process_array
   la   $a0, arr1
   jal  process_array
   la   $a0, arr2
   jal  process_array
   la   $a0, arr3
   jal  process_array
   la   $a0, overflow
   jal  process_array

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   jr    $ra                # return to kernel
## end main ################################################

# process_array(uint* arr): #################################
#   print_array(arr)
#   reverse_prefix_sum(arr)
#   print_array(arr)
#
process_array:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   subu $sp, $sp, 4        # save s0 on stack before using it
   sw   $s0, 4($sp)

   move $s0, $a0           # use s0 to save a0
   jal  print_array
   move $a0, $s0
   jal  reverse_prefix_sum
   move $a0, $s0
   jal  print_array

   lw   $s0, -8($fp)       # restore s0 from stack

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   jr    $ra                # return to kernel
## end process_array #######################################

# print_array(uint arr): ########################################
#   uint x
#   while (-1 != (x = *arr++)):
#     printf("%d ", x)
#   printf("\n")
#
print_array:
   # use t0 to hold arr. use t1 to hold *arr
   move $t0, $a0
print_array_while:
   lw   $t1, ($t0)
   beq  $t1, -1, print_array_endwhile
   move $a0, $t1           # print_int(*arr)
   li   $v0, 1
   syscall
   li   $a0, 32            # print_char(' ')
   li   $v0, 11
   syscall
   addu $t0, $t0, 4
   b    print_array_while
print_array_endwhile:
   li   $a0, 10            # print_char('\n')
   li   $v0, 11
   syscall
   jr   $ra
## end print_array #########################################
reverse_prefix_sum:
   # PROLOGUE
   subu	$sp, $sp, 8
   sw	$ra, 8($sp)
   sw	$fp, 4($sp)
   addu	$fp, $sp, 8

   # BODY
   lw $t0, ($a0) # load r (array entry)
   subu $sp, $sp, 8

   bne	$t0, -1, rec     # while entry != -1
   li	$v0, 0    # if *arr == -1, return 0
   j	ret

# a = [ 1, 2, 3, 4, -1]
# b = [10, 9, 7, 4]
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
    sw 	$a0, 8($sp) # store curr arr pointer
    sw $t0, 4($sp) # save r, $t0 = r
    addu $a0, $a0, 4 # go to next entry 
    jal  reverse_prefix_sum 		# call reverse prefix sum 

    lw $a0, 8($sp) # load a0
    lw	$t0, 4($sp)	# load r

    addu	$v0, $v0, $t0  	# $v0 = adds returned recursive value to curr value
    sw $v0, ($a0) # store the value in array 

    j ret
    
ret:
    # EPILOGUE
    move	$sp, $fp
    lw	$ra, ($fp)
    lw	$fp, -4($sp)
    jr 	$ra
