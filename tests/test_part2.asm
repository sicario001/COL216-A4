addi $s00, $zero, 1000
addi $t2, $zero, 1
sw $t2, 0($s00)
lw $s0, 0($s00)
addi $s1, $s1, 1
add $s0, $s0, $s1
