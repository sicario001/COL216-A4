main:
    addi $s0, $zero, 10

    addi $s1, $zero, 1

    slt $s4, $s0, $s1
    beq $s4, $s1, exit1

    addi $s1, $s1, +2
    addi $s2, $zero, 1

    slt $s4, $s0, $s1
    beq $s4, $s2, exit1
    add $s1, $zero, $s2

    addi $s0, $s0, -2
loop:
    add $s3, $s1, $s2
    addi $s0, $s0, -1
    add $s1, $s2, $zero
    add $s2, $s3, $zero
    bne $s0, $zero, loop
    j exit2
exit1:
    addi $s3, $zero, 1
exit2:
    add $s3, $s3, $zero
