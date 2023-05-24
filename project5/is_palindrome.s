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
   