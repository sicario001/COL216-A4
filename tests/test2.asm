main:
    addi $s0, $zero, 10
    addi $s1, $zero, 1
loop:
    mul $s1, $s1, $s0
    addi $s0, $s0, -1
    bne $zero, $s0, loop
exit:
