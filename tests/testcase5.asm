	addi $s3, $zero, 1
	addi $s4, $zero, 2
	sw $s3, 1000
	sw $s4, 1024
	lw $s5, 1000
	lw $s6, 1024
	add $s6, $s6, $s5
	add $s6, $s6, $s5
	add $s6, $s6, $s5
	add $s6, $s6, $s5
	sw $s6, 1028
	sw $s6, 3028
	sw $s6, 1032
	sw $s6, 3032
	sw $s6, 1024
	
