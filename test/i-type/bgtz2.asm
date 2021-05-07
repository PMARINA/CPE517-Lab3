addi $t1, $zero, 2
bgtz $t1 test
addi $t5, $t5, 1
addi $t5, $t5, 1
addi $t5, $t5, 1
test:
addi $t1, $t1, -1
addi $t0, $zero, 5
bgtz $t1, test

# To verify this test,
# rdump
# $8 = 5
# $9 = 0
# $13 = 0, if not 0, the addi were not properly skipped
