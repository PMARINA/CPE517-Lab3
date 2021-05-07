.text

load:
  addi $t0 ,$zero , 3
  addi $t2, $zero, -3

compute:
  addi $v0 ,$zero ,1
  addiu $a0, $t0, 5
  syscall
  addiu $a0, $t0, -1
  syscall
  addiu $a0, $t0, 65535
  syscall

  addiu $a0, $t2, 5
  syscall
  addiu $a0, $t2, -1
  syscall
  addiu $a0, $t2, 65535
  syscall

  addiu $v0, $zero, 10
  syscall


# Expected outputs
# 8
# 2
# 65538

# 2
# -4
# 65532

# NOTE: MARS, as of this moment, uses an addiu that is capable of holding immediate values larger than the 2^16 capable of being represented in a single instruction, per the reference guides.
# Also, MARS is not doing unsigned representation for the immediate value (negative immediates are treated as negative values)
