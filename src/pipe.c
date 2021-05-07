#include "pipe.h"
#include "shell.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AND 0
#define OR 1
#define ADD 2
#define LESSU 3
#define NOP 5
#define SUB 6
#define LESS 7

/*global pipeline data*/
Pipe_Reg_IFtoDE Reg_IFtoDE;
Pipe_Reg_DEtoEX Reg_DEtoEX;
Pipe_Reg_EXtoMEM Reg_EXtoMEM;
Pipe_Reg_MEMtoWB Reg_MEMtoWB;

// for forwarding
int forwardA = 0;
int forwardB = 0;
int stall = 0;
void pipe_cycle() {
  pipe_stage_wb();
  pipe_stage_mem();
  pipe_stage_execute();
  pipe_stage_decode();
  pipe_stage_fetch();
}

/*
 * every instruction is 5 stages.
 * wait times is 5, and the last instruction's execution is over.
 */
void pipe_stage_fetch() {
  if (stall > 0) // if stall, not need to load instruction
  {
    stall--;
    return;
  }
  uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
  static int times = 0; // static variable - initialize once
  if (instruction == 0) {
    times++;
    if (times == 5)
      RUN_BIT = 0;
  } else
    times = 0;
  CURRENT_STATE.PC = CURRENT_STATE.PC + 4; // set PC
  Reg_IFtoDE.ins1 = instruction;
  Reg_IFtoDE.PC = CURRENT_STATE.PC;
}

/*
 * return a signed int saved as a unsigned number
 */
uint32_t signExtend16to32(uint32_t val16) {
  if (val16 & 0x8000)
    return val16 | 0xffff0000; // signed extend
  else
    return val16;
}
uint32_t getOp(uint32_t instruction) { return instruction >> 26 & 0x3f; }
uint32_t getFunct(uint32_t instruction) { return instruction & 0x3f; }
uint32_t regDst(uint8_t ex) { return ex & 0b1000; }
uint32_t aluSrc(uint8_t ex) { return ex & 0b0001; }
uint32_t memRead(uint8_t m) { return m & 0b0010; }
uint32_t memWrite(uint8_t m) { return m & 0b0001; }
uint32_t regWrite(uint8_t wb) { return wb & 0b0010; }
uint32_t memtoReg(uint8_t wb) { return wb & 0b0001; }
uint32_t branch(uint8_t m) { return m & 0b0100; }
uint32_t jump(uint8_t m) { return m & 0b1000; }
// detect forwarding
void detect_hazard() {
  forwardA = 0;
  forwardB = 0;
  // EX hazard

  if (regWrite(Reg_EXtoMEM.wb_mem >> 4) && Reg_EXtoMEM.addr_rd != 0) {
    if (Reg_EXtoMEM.addr_rd == Reg_DEtoEX.addr_rs) {
      forwardA = 2; // 0b:10
    }
    if (Reg_EXtoMEM.addr_rd == Reg_DEtoEX.addr_rt) {
      forwardB = 2; // 0b:10
    }
  }
  // MEM hazard
  if (regWrite(Reg_MEMtoWB.wb) && Reg_MEMtoWB.addr_rd != 0) {
    if (Reg_MEMtoWB.addr_rd == Reg_DEtoEX.addr_rs) {
      forwardA = 1;
    }
    if (Reg_MEMtoWB.addr_rd == Reg_DEtoEX.addr_rt) {
      forwardB = 1;
    }
  }
}
int check_stall(uint32_t rs, uint32_t rt) {
  if (memRead(Reg_DEtoEX.wb_mem) &&
      (Reg_DEtoEX.addr_rt == rs || Reg_DEtoEX.addr_rt == rt)) {
    return 1;
  }
  return 0;
}
void pipe_stage_decode() {
  printf("\nthis is decode!!!");
  uint32_t instruction = Reg_IFtoDE.ins1;

  if (instruction == 0) {
    printf("there is no instruction need to decode!\n");
    Reg_DEtoEX.ex = 0; // clear control line
    Reg_DEtoEX.wb_mem = 0;
    return;
  }

  uint8_t addr_rs = (instruction >> 21) & 0x1f;
  uint8_t addr_rt = (instruction >> 16) & 0x1f;
  uint8_t addr_rd = (instruction >> 11) & 0x1f;

  stall += check_stall(addr_rs, addr_rt);

  if (stall) {
    printf("encounter lw and need a stall\n");
    Reg_DEtoEX.ex = 0; // clear control line
    Reg_DEtoEX.wb_mem = 0;
    return;
  }

  Reg_DEtoEX.ins = instruction; // transfer the instruction
  Reg_DEtoEX.rs_value = CURRENT_STATE.REGS[addr_rs];
  Reg_DEtoEX.rt_value = CURRENT_STATE.REGS[addr_rt];
  // register number
  Reg_DEtoEX.addr_rt = addr_rt;
  Reg_DEtoEX.addr_rd = addr_rd;
  Reg_DEtoEX.addr_rs = addr_rs;
  Reg_DEtoEX.inum = signExtend16to32(instruction & 0x0000ffff);
  Reg_DEtoEX.PC = Reg_IFtoDE.PC;
  // Below is to set wb mem and ex
  // each is one nibble
  // ex : RegDst ALUOp1  ALUOP0    ALUSrc
  // m  :  jump   Branch  MemRead   MemWrite
  // wb :   X      X     RegWrite  MemtoReg
  uint32_t op = getOp(instruction);
  if (op == 0) {                      // R type
    Reg_DEtoEX.wb_mem = (2 << 4) | 0; // wb=0010, m=0000
    Reg_DEtoEX.ex = 12;               // ex=1100
  } else if (op == 0x23) {            // lw
    Reg_DEtoEX.wb_mem = (3 << 4) | 2; // wb=0011, m=0010
    Reg_DEtoEX.ex = 1;                // ex=0001
  } else if (op == 0x2b) {            // sw
    Reg_DEtoEX.wb_mem = 0 | 1;        // wb=0000, m=0001
    Reg_DEtoEX.ex = 1;                // ex=0001
    // Implemented code:
  } else if (op == 0x4 || op == 0x5 || op == 0x7) { // beq, bne, bgtz
    Reg_DEtoEX.wb_mem = 0 | 4;                      // wb=0000, m=0100
    Reg_DEtoEX.ex = 0;                              // ex=0000
  } else if (op == 0x2) {                           // j
    Reg_DEtoEX.wb_mem = 0 | 8;
    Reg_DEtoEX.ex = 0;
  } else {                            // slti, lui, ori, addi, addiu
    Reg_DEtoEX.wb_mem = (2 << 4) | 0; // wb=0010, m=0000
    Reg_DEtoEX.ex = 7;                // ex=0111
  }

  detect_hazard();
}

uint8_t ALU_control(uint32_t instruction) { // calculate the ALU control input

  uint32_t op = getOp(instruction);
  uint32_t funct = getFunct(instruction);

  uint8_t control = -1;
  if (op == 0) {
    switch (funct) {
    case 0x20: // add
    case 0x21: // addu
      control = ADD;
      break;
    case 0x22: // sub
    case 0x23: // subu
      control = SUB;
      break;
      /*Implement other cases here*/
    case 0x2a: // slt
      control = LESS;
      break;
    case 0x2b: // sltu
      control = LESSU;
      break;
    default:
      printf("ALU Control - Unknown func received: %x", funct);
      break;
    }
  } else {
    switch (op) {
    case 0x23: // lw
    case 0x2b: // sw
    case 0xf:  // lui
    case 0x8:  // addi
    case 0x9:  // addiu
      control = ADD;
      break;
    case 0x4: // beq
    case 0x5: // bne
      control = SUB;
      break;
    case 0x7: // bgtz
    case 0xa: // slti
      control = LESS;
      break;
    case 0xd: // ori
      control = OR;
      break;
    case 0x2: // j
      control = NOP;
      break;
    default:
      printf("ALU Control - Unknown op received: %x", op);
      break;
    }
  }
  return control;
}
uint32_t getInput1() {
  uint32_t value = Reg_DEtoEX.rs_value;
  if (forwardA == 2) {
    value = Reg_EXtoMEM.AluResult;
  } else if (forwardA == 1) {
    if (memtoReg(Reg_MEMtoWB.wbCopy) == 0) // for lw,  memtoReg is 1.
      value = Reg_MEMtoWB.AluResultCopy;
    else // In WB stages , rt_value is save data from memory
      value = Reg_MEMtoWB.rt_value;
  }
  return value;
}
uint32_t getInput2() {
  uint32_t value = Reg_DEtoEX.rt_value;
  if (forwardB == 2) {
    value = Reg_EXtoMEM.AluResult;
  } else if (forwardB == 1) {
    if (memtoReg(Reg_MEMtoWB.wbCopy) == 0) // for lw,  memtoReg is 1.
      value = Reg_MEMtoWB.AluResultCopy;
    else // In WB stages , rt_value is save data from memory
      value = Reg_MEMtoWB.rt_value;
  }
  return value;
}
void pipe_stage_execute() {
  printf("\nthis is execute!!!");

  uint32_t op = getOp(Reg_DEtoEX.ins);
  uint32_t funct = getFunct(Reg_DEtoEX.ins);

  if (CURRENT_STATE.REGS[2] == 10 && op == 0 && funct == 0xc) {
    pipe_stage_wb();
    pipe_stage_mem();
    // pipe_stage_wb();
    RUN_BIT = FALSE;
  }

  // pass wb and mem
  Reg_EXtoMEM.wb_mem = Reg_DEtoEX.wb_mem;
  if (Reg_EXtoMEM.wb_mem == 0) { // no need to continue
    return;
  }
  // calculate new PC for branch
  Reg_EXtoMEM.newPC1 = Reg_DEtoEX.PC + (Reg_DEtoEX.inum << 2);
  // calculate new PC for jump
  Reg_EXtoMEM.newPC2 = (Reg_DEtoEX.PC & 0xf0000000) | (Reg_DEtoEX.inum << 2);
  // pass rt value
  Reg_EXtoMEM.rt_value = Reg_DEtoEX.rt_value;
  // pass addr_rd
  if (regDst(Reg_DEtoEX.ex) != 0) { // RegDst=1 rd
    Reg_EXtoMEM.addr_rd = Reg_DEtoEX.addr_rd;
  } else {
    Reg_EXtoMEM.addr_rd = Reg_DEtoEX.addr_rt; // RegDst=0 rt
  }

  uint32_t input1 = getInput1(); // Reg_DEtoEX.rs_value;
  uint32_t input2 = getInput2(); // Reg_DEtoEX.rt_value;

  if (aluSrc(Reg_DEtoEX.ex) != 0) { // aluscr means use inum
    input2 = Reg_DEtoEX.inum;
  }

  if (op == 0xf) // for lui
    input2 = input2 << 16;
  else if (op == 7) // for bgtz
  {
    input2 = input1;
    input1 = 0;
  }

  uint8_t control = ALU_control(Reg_DEtoEX.ins); // control for alu
  switch (control) {
  case ADD:
    Reg_EXtoMEM.AluResult = input1 + input2;
    break;
  case SUB:
    Reg_EXtoMEM.AluResult = input1 - input2;
    break;
  case AND:
    Reg_EXtoMEM.AluResult = input1 & input2;
    break;
  case OR:
    Reg_EXtoMEM.AluResult = input1 | input2;
    break;
  case LESS:
    if ((int)input1 < (int)input2)
      Reg_EXtoMEM.AluResult = 1;
    else
      Reg_EXtoMEM.AluResult = 0;
    break;
  case LESSU:
    if (input1 < input2)
      Reg_EXtoMEM.AluResult = 1;
    else
      Reg_EXtoMEM.AluResult = 0;
    break;
  default:
    break;
  }
  Reg_EXtoMEM.zero = !Reg_EXtoMEM.AluResult; // set zero flag
  if (op == 5 || op == 7)                    // for bne or bgtz
    Reg_EXtoMEM.zero = !Reg_EXtoMEM.zero;
}
void fflush_instruction() {
  Reg_EXtoMEM.wb_mem = 0;
  Reg_DEtoEX.ex = 0;
  Reg_DEtoEX.wb_mem = 0;
  Reg_IFtoDE.ins1 = 0;
}
void pipe_stage_mem() {
  printf("\nthis is mem!!!\n");
  Reg_MEMtoWB.wbCopy = Reg_MEMtoWB.wb;
  Reg_MEMtoWB.wb = Reg_EXtoMEM.wb_mem >> 4;
  Reg_MEMtoWB.addr_rd = Reg_EXtoMEM.addr_rd;
  // make a copy for forwarding
  Reg_MEMtoWB.AluResultCopy = Reg_MEMtoWB.AluResult;
  Reg_MEMtoWB.AluResult = Reg_EXtoMEM.AluResult;
  // rt value actually read data from memory
  Reg_MEMtoWB.rt_value = Reg_EXtoMEM.rt_value;
  /*Implement your code here*/
  if (memWrite(Reg_EXtoMEM.wb_mem) != 0) {
    // Need to write
    mem_write_32(Reg_MEMtoWB.AluResult, Reg_MEMtoWB.rt_value);
  }
  if (memRead(Reg_EXtoMEM.wb_mem) != 0) {
    // Need to read
    Reg_MEMtoWB.rt_value = mem_read_32(Reg_MEMtoWB.AluResult);
  }
  if (branch(Reg_EXtoMEM.wb_mem) != 0) {
    // Branch
    fflush_instruction();
    CURRENT_STATE.PC = Reg_EXtoMEM.newPC1;
  }
  if (jump(Reg_EXtoMEM.wb_mem) != 0) {
    // Jump
    fflush_instruction();
    CURRENT_STATE.PC = Reg_EXtoMEM.newPC2;
  }
}

/*
 * if regWrite = 0, mem/wb will not run
 */
void pipe_stage_wb() { /*Implement your code here*/
  uint32_t val_to_write = 0;
  if (memtoReg(Reg_MEMtoWB.wb)) {
    // need to write from mem to reg
    val_to_write = Reg_MEMtoWB.rt_value;
  } else {
    // need to write from ALU to reg
    val_to_write = Reg_MEMtoWB.AluResult;
  }
  if (regWrite(Reg_MEMtoWB.wb) != 0) {
    // need to write register (not a jump/branch/store instruction)
    CURRENT_STATE.REGS[Reg_MEMtoWB.addr_rd] = val_to_write;
  }
}
