   .data
str1:
   .asciiz "abba"
str2:
   .asciiz "racecar"
str3:
   .asciiz "swap paws",
str4:
   .asciiz "not a palindrome"
str5:
   .asciiz "another non palindrome"
str6:
   .asciiz "almost but tsomla"

# array of char pointers = {&str1, &str2, ..., &str6}
ptr_arr:
   .word str1, str2, str3, str4, str5, str6, 0

yes_str:
   .asciiz " --> Y\n"
no_str:
   .asciiz " --> N\n"

   .text

# main(): ##################################################
#   char ** j = ptr_arr
#   while (*j != 0):
#     rval = is_palindrome(*j)
#     printf("%s --> %c\n", *j, rval ? yes_str: no_str)
#     j++
#
main:
   li   $sp, 0x7ffffffc    # initialize $sp

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   subu $sp, $sp, 8        # save s0, s1 on stack before using them
   sw   $s0, 8($sp)        # push $s0
   sw   $s1, 4($sp)        # push $s1

   la   $s0, ptr_arr        # use s0 for j. init ptr_arr
main_while:
   lw   $s1, ($s0)         # use s1 for *j
   beqz $s1, main_end      # while (*j != 0):
   move $a0, $s1           #    print_str(*j)
   li   $v0, 4
   syscall
   move $a0, $s1           #    v0 = is_palindrome(*j)
   jal  is_palindrome
   beqz $v0, main_print_no #    if v0 != 0:
   la   $a0, yes_str       #       print_str(yes_str)
   b    main_print_resp
main_print_no:             #    else:
   la   $a0, no_str        #       print_str(no_str)
main_print_resp:
   li   $v0, 4
   syscall

   addu $s0, $s0, 4       #     j++
   b    main_while        # end while
main_end:

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   jr    $ra                # return to kernel
# end main ################################################
# Kristin Chen, 118035390, kchen14
# int: i, strlen, end of str = strlen - 1
# for (i < len / 2)
# if (str[i] != str[strlen - i]
# return 0 false
# return 1 true

is_palindrome:

   # PROLOGUE
   subu $sp, $sp, 8    
   sw   $ra, 8($sp)     
   sw   $fp, 4($sp)       
   addu $fp, $sp, 8 

   # BODY
   jal strlen
   move $t0, $v0        # store strlen into t0
   # v0 (now a0) is pointing to end of str from strlen
   move $a1, $a0        # for beg of str
   move $a2, $a0        # end of str
   sub $a1, $a1, $t0    # beg of str pointer (end of string - end of string)
   sub $a2, $a2, 1      # end of str pointer = strlen - 1

   # t0 strlen / 2
   # t1 i
   # a1 beg of str
   # a2 end of str
   div $t0, $t0, 2      # strlen / 2, for loop
   add $t0, $t0, 1      # add 1 for <=
   li $t1, 0            # i set to 0 for loop
   li $v0, 1            # true

   p_loop: # a0 is where string is located
   # t2, t3 temp holds byte from beg and end of str
   lb $t2, ($a1)              # load first byte of word
   lb $t3, ($a2)              # load last byte
   beq $t1, $t0, p_endloop    # check if strlen is reached
   beq $t2, $t3, p_increment  # go to next set of chars if same
   li $v0, 0 # false
   bne $t2, $t3, p_endloop    # endloop if diff chars

p_increment:
   addi $a1, $a1, 1           # incrementing beg pointer
   sub $a2, $a2, 1            # decrementing end pointer
   addi $t1, $t1, 1           # incrementing t0 (i)
   j p_loop
   
p_endloop:
   # EPILOGUE
   move $sp, $fp           
   lw   $ra, ($fp)         
   lw   $fp, -4($sp)      
   jr    $ra  

# strlen(str) ==================================================================

strlen:
   # PROLOGUE
   subu $sp, $sp, 8    
   sw   $ra, 8($sp)     
   sw   $fp, 4($sp)       
   addu $fp, $sp, 8    
   # BODY
   li $t0, 0            # t0 set to 0 for count

loop: # a0 is where string is located
   lb $t1, 0($a0)
   beqz $t1, endloop    # check if is nul-terminate
   addi $a0, $a0, 1     # incrementing pointer
   addi $t0, $t0, 1     # incrementing t0 (i)
   j loop

endloop:
   move $v0, $t0
   # EPILOGUE
   move $sp, $fp           
   lw   $ra, ($fp)         
   lw   $fp, -4($sp)      
   jr    $ra  
   
