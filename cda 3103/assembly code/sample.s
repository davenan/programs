# Dave Nandlall
# CDA 3103
# Homework #3 - MIPS Programming Assignment
# July 3rd, 2013
#
# Text Segment
#

        .text
	.ent main
	
main:		
		li $v0, 4       # load argument 4 into register $v0
		
        la $a0, str1  	# load address of str1 in register $a0
        
		syscall			# syscall to print the string that has to be tested
		
		j NumSpaces	    # jump to function numspaces
		
		

.ent NumSpaces 			# begin function numspaces

NumSpaces:       
    
    li $t0, 0			# load the counter for the spaces into $t0 (count = 0)
    
    li $t1, 32			# load the variable SPACE into $t1 denoted by asciiz value SPACE = 32 (t1 = ' ')       
    
    
loop: 
	
	lbu $t2, 0($a0)			# load first byte from string into $t2 
	
	beq $t2, $0, exit		# test if the byte is null character, if so branch to exit loop
	
	bne $t2, $t1, this	 	# else test if byte is a SPACE character, if its not a SPACE jump to this label
							# else just go to next instruction
	
	add $t0, $t0, 1			# if the character of the string "IS" a SPACE add 1 to our counter in $t0 (t0++)
	
	add $a0, $a0, 1			# add 1 to $a0 to load next character in the string (t -> th -> the -> ther ....)
		
	j loop					# jump back to our loop and repeat the instructions
	
	
this: 	

	add $a0, $a0, 1         # add 1 to go to the next character in the string, if it is NOT a SPACE
	
	j loop					# jump back to loop label to test the character in the string 


exit:

	  li $v0, 1				# when null character is reached in our string, load integer arg 1 into $v0
	  
	  move $a0, $t0			# perform a move operation, to move our counter variable with the number of spaces
	  						# that were present in the string to $a0 our argument register
	  						
	  syscall				# perform syscall to print our answer that is the integer in $t0
	  
	  jr $31				# return to main
        	
        	
	.end NumSpaces		    # end numspaces function
	
.end main					# end main function
	
	
#
# Data Segement
#

	.data
str1:   .asciiz "hello there i am a string with 8 spaces\n"
