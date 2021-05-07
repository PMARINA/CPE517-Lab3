.text
# Test 1 -- no special things. Just write and read to the same place. 
printsetup:
	addi $v0, $zero, 34 # Print int in hex
addrsetup1:
	lui $t5, 0x1001
setup1:
	lui $t0, 0xABCD
	addi $t0, $t0, 0x1234
	sw $t0, 0($t5)
test1:
	addi $a0, $a0, 0
	lw $a0, 0($t5)
	syscall
	
# Test 2 -- read and write with positive offset.
addrsetup2:
	lui $t5, 0x1001
	addi $t5, $t5, 0x0008
setup2:
	lui $t0, 0xABCD
	addi $t0, $t0, 0x1234
	sw $t0, 8($t5)
test2:
	addi $a0, $a0, 0
	lw $a0, 8($t5)
	syscall

# Test 3 -- read and write with positive offset.
addrsetup3:
	lui $t5, 0x1001
	addi $t5, $t5, 0x00024
setup3:
	lui $t0, 0xABCD
	addi $t0, $t0, 0x1234
	sw $t0, -8($t5)
test3:
	addi $a0, $a0, 0
	lw $a0, -8($t5)
	syscall

# Expected outputs
# You should see 3 hex numbers printed among the various state-prints
#
# Also in memory...
# mdump 0x10010000 0x1001001c
# All others except specified should be 0
# 0x10010000: (0xABCD1234)
# 0x10010010: (0xABCD1234)
# 0x1001001c: (0xABCD1234)