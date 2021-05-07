.text
load:
	addi	$v0, $zero, 1
	addi	$t0, $zero, 0
	addi	$t1, $zero, 5
	addi	$t2, $zero, -5
compute:
	add		$a0, $t0, $t1
	syscall

	add		$a0, $t1, $t2
	syscall

	add		$a0, $t0, $t2
	syscall

	
addi	$v0, $zero, 10
syscall

# Expected outputs:
# 5
# 0
# -5
