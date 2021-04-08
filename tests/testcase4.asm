addi $s0, $zero, 2000
addi $t2, $zero, 100
sw $t2, 0($s0)
addi $s1, $zero, 3000
addi $t0, $zero, 200
sw $t0, 0($s1)
addi $s2, $zero, 4000
addi $t1, $zero, 300
sw $t1, 0($s2)
addi $s3, $zero, 5000
addi $t3, $zero, 400
sw $t3, 0($s3)
lw $t4, 0($s0)
lw $t5, 0($s1)
lw $t6, 0($s2)
lw $t7, 0($s3)
