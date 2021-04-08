	addi $s0, $zero, 1000
	addi $s1, $zero, 1024
	addi $s3, $zero, 1
	addi $s4, $zero, 2
	sw $s3, 0($s0)
	sw $s4, 0($s1)
	lw $s5, 0($s0)
	lw $s6, 0($s1)
	add $s6, $s6, $s5
	addi $s2, $zero, 1028
	sw $s6, 0($s2)
