# Authors: Julia, Pridhvi
lui $t0, 0x1001
addi $t1, $zero, 0xA
sw $t1, ($t0)
lw $t2, ($t0)
add $t3, $zero, $t2
addi $t4, $zero, 0xB
addi $t5, $zero, 0xC
addi $t6, $zero, 0xD
addi $t7, $zero, 0xE
add $v0, $t1, $zero
syscall