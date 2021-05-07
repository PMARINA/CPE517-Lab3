.text
addi	$t0, $zero, 4
addi	$t1, $zero, 5
jal		perimeter
addi	$a0, $zero, 0
addi	$v0, $zero, 1	#returning integer
syscall
jal skip
addi $v0, $zero, 10
syscall

perimeter:
add		$t2, $t0, $t0
add		$t3, $t1, $t1
add		$a0, $t2, $t3
addi	$v0, $zero, 1	#returning integer
syscall
jr    $ra

skip:
add $a0, $t0, $t1
addi $v0, $zero, 1
syscall
jr    $ra

addi  $a0, $zero, 5
addi	$v0, $zero, 1	#returning integer
syscall
addi $v0, $zero, 10
syscall


# Expected outcome:
# 18 0 9 {exit}
# if fail:
# If you see a 5 or see things in a different order...
