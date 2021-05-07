.text
load:
	addi $v0, $zero, 1
	addi $t0, $zero, 2
	addi $t1, $zero, 5
	addi $t2, $zero, -1 #65534
	addi $t3, $zero, -10 #65525
compute:
	subu $a0, $t1, $t0
	syscall
	subu $a0, $t0, $t1
	syscall
	subu $a0, $t2, $t1
	syscall
	subu $a0, $t2, $t3
	syscall

# Expected outputs:
# 3
# -3
# -6
# 9
