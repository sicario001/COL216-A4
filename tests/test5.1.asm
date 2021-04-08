main:
    addi $s0, $zero, 1000
    addi $s2, $zero, 28

    j ($s2)
    save:
        sw $s1, ($s0)
        addi $s0, $s0, 4
        addi $s2, $s2, 8
        j ($s2)

    addi $s1, $zero, 100
    j save

    addi $s1, $zero, 290
    j save

    addi $s1, $zero, 150
    j save

    add $t2, $zero, $s0

    addi $s0, $zero, 1000

    add $s3, $zero, $zero

    addi $t0, $s2, 48
    addi $t1, $s2, 20

    beq $s0, $t2, ($t0)
    lw $s5, ($s0)
    addi $s0, $s0, 4
    slt $s6, $s5, $s3
    bne $s6, $zero, ($t1)
    add $s3, $zero, $s5
    j ($t1)
exit:
    add $s3, $zero, $s3
