#  Author: Jake Foglia
.text:
  addi $t0 $zero 0x1
  lui $a0 0x1001
  #ori $a0 $a0 0x8

  sw $t0 0x0($a0)
  lw $t1 0x0($a0)       

  addi $v0 $zero 10
  syscall
