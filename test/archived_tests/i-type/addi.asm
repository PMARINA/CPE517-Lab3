.text
load:
  addi $t0 ,$zero , 3
  addi $t2, $zero, -3

compute:
  addi $v0 ,$zero ,1
  addi $a0, $t0, 5
  syscall
  addi $a0, $t0, -5
  syscall
  addi $a0, $t0, -2
  syscall
  addi $a0, $t0, -3
  syscall

  addi $a0, $t2, 5
  syscall
  addi $a0, $t2, 1
  syscall
  addi $a0, $t2, -5
  syscall
  addi $a0, $t2, -1
  syscall

  addi $v0, $zero, 10
  syscall


# Expected outputs
# 8
# -2
# 1
# 0

# 2
# -2
# -8
# -4
