.text

addi $t0, $zero, 0xfa
lui $t1, 0x1001
addi $t1, $t1, 4
sb $t0, -1($t1)
add $t0, $zero, $zero

lb $t0, -1($t1)
addi $t0, $t0, 1
sb $t0, -1($t1)
addi $a0, $zero, 0
lb $a0, -1($t1)
addi $v0, $zero, 1
syscall

addi $t0, $zero, 0xfa
lui $t1, 0x1001
addi $t1, $t1, 24
sb $t0, -1($t1)
add $t0, $zero, $zero

lb $t0, 1($t1)
addi $t0, $t0, 1
sb $t0, 1($t1)
addi $a0, $zero, 0
lb $a0, 1($t1)
addi $v0, $zero, 1
syscall

addi $t0, $zero, 0xfa
lui $t1, 0x1001
addi $t1, $t1, 32
sb $t0, 1($t1)
add $t0, $zero, $zero

lb $t0, 1($t1)
addi $t0, $t0, 1
sb $t0, 1($t1)
addi $a0, $zero, 0
lb $a0, 1($t1)
addi $v0, $zero, 1
syscall

addi $t0, $zero, 0x03
sb $t0, 0x10010000
add $t0, $zero, $zero

lb $t0, 0x10010000
addi $t0, $t0, 1
sb $t0, 0x10010000
addi $a0, $zero, 0
lb $a0, 0x10010000
addi $v0, $zero, 1
syscall

addi $t0, $zero, 0x02
sb $t0, 0x10010000
add $t0, $zero, $zero

lb $t0, 0x10010000
addi $t0, $t0, 1
sb $t0, 0x10010000
addi $a0, $zero, 0
lb $a0, 0x10010000
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

# Expected output
# -5
# 1
# -5
# 4
# 3
