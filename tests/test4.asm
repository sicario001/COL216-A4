main:
    addi $t0, $zero, 1000

    addi $t1, $zero, 1
    addi $t2, $zero, 0
    addi $t3, $zero, 20

    sw $t1, ($t0)
loop:
    lw $t4, ($t0)
    add $t1, $t4, $t4
    sw $t1, 4($t0)
    addi $t0, $t0, 4
    addi $t2, $t2, 1
    bne $t2, $t3, loop
exit:
