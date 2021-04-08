main:
	
	addi $s0, $zero, 2000
	addi $s1, $zero, 2000
	
	beq $s0, $s1, label1
	
	label1 : 
		addi $s0, $s0, -1000
		
	bne $s0, $zero, label1
	beq $s0, $zero, label2
	
	label2 :
		
exit: 
