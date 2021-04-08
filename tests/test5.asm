addi $s0, $zero, 1000

addi $s1, $zero, 100
addi $s2, $zero, 16
j 40

addi $s1, $zero, 290
addi $s2, $zero, 28
j 40

addi $s1, $zero, 150
addi $s2, $zero, 52
j 40

sw $s1, ($s0)
addi $s0, $s0, 4
j ($s2)

add $t2, $zero, $s0

addi $s0, $zero, 1000

add $s3, $zero, $zero

beq $s0, $t2, 92
lw $s5, ($s0)
addi $s0, $s0, 4
slt $s6, $s5, $s3
bne $s6, $zero, 64
add $s3, $zero, $s5
j 64

add $s3, $zero, $s3
