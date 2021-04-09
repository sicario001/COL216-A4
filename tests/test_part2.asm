addi $s2, $zero, 120
addi $t2, $zero, 1
sw $t2, 0($s2)
lw $s0, 0($s2)
addi $s1, $s1, 1
add $s0, $s0, $s1
