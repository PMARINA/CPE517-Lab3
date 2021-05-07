.text
addi	$t0, $zero, 4
addi	$t1, $zero, 5
j		perimeter
skip:
addi	$a0, $zero, 0
addi	$v0, $zero, 1	#returning integer
syscall
addi $v0, $v0, 9
syscall

perimeter:
add		$t2, $t0, $t0
add		$t3, $t1, $t1
add		$a0, $t2, $t3

addi	$v0, $zero, 1	#returning integer
syscall
j skip

# Expected outcome:
# 180
# if fail:
# 018, etc.
