.text

addi $t0, $zero, 1
bgtz $t0, skipadd
addi $t0, $t0, 1

skipadd:
  add $a0, $zero, $t0

addi $v0, $zero, 1
syscall

# Expected Output
# If branch works correctly
# 1
# If it fails,
# 2

add $t0, $zero, $zero
bgtz $t0, skipadd1
addi $t0, $t0, 1

skipadd1:
  add $a0, $zero, $t0

addi $v0, $zero, 1
syscall
# Expected Output
# If branch works correctly
# 1
# If it fails,
# 0

addi $t0, $zero, -1
bgtz $t0, skipadd2
addi $t0, $t0, 1

skipadd2:
  add $a0, $zero, $t0

addi $v0, $zero, 1
syscall
# Expected Output
# If branch works correctly
# 0
# If it fails,
# -1

addi $v0, $zero, 10
syscall

# Expected output
# 1 1 0
