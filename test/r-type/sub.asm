.text
load:
	addi $t0, $zero, 2
	addi $t1, $zero, 5
	addi $t2, $zero, -1
	addi $t3, $zero, -10
compute:
	addi $v0, $zero, 1
	sub $a0, $t1, $t0
	syscall
	sub $a0, $t0, $t1
	syscall
	sub $a0, $t1, $t2
	syscall
	sub $a0, $t2, $t3
	syscall

	addi $v0, $zero, 10
	syscall

# Expected outputs:
# 3
# -3
# 6
# 9
