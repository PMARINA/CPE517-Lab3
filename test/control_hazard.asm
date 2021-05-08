# Authors: Julia, Pridhvi
addi $t0, $zero, 0x0
bne $t0,$zero, .branch_test
addi $t1, $zero, 0xcd
addi $t2, $zero, 0xef
addi $t3, $zero, 0x7
addi $t4, $zero, 0x8
.branch_test:
  addi $t5, $zero, 0xab
  addi $t6, $zero, 0x12
addi $v0, $zero, 10
syscall

# Test passes if you check rdump and correct values are found
# To test the various types of branch, replace bne with other
# branch instructions and test both conditions.