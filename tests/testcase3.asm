addi $s0, $zero, 1000
addi $t0, $zero, 100
sw $t0, 0($s0)
addi $s1, $zero, 5
addi $s2, $s1, -5
sub $s2, $s2, $s1
addi $s3, $s2, 50
mul $s1, $s2, $s3
lw $t2, 0($s0)
add $t1, $t2, $s1
