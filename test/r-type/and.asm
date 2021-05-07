.text
load:
	addi $t0, $zero, 0x11111111
	addi $t1, $zero, 0x00000000
	addi $t2, $zero, 0x11110000

compute:
	addi $v0, $zero, 1
	and $a0, $t0, $t1
	syscall
	and $a0, $t0, $t2
	syscall
	and $a0, $t1, $t1
	syscall


addi	$v0, $zero, 10
syscall

# Expected outputs:
# 0
# 286326784
# 0
