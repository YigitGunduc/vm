#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

enum {
  //MOVE INSTRUCTIONS
  MOVE_LIT_TO_REG = 0x10,
  MOVE_REG_TO_REG,
  MOVE_REG_TO_MEM,
  MOVE_MEM_TO_REG,
  MOVE_LIT_TO_MEM,

  // ARITHMETICS
  ADD_REG_TO_REG,
  ADD_LIT_TO_REG,
  SUB_LIT_TO_REG,
  SUB_REG_TO_REG,
  INC_ACC,
  DEC_ACC,
  INC_REG,
  DEC_REG,

  //MAPPED MEMORY INSTRUCTIONS
  MOVE_MMEM_TO_REG,
  MOVE_REG_TO_MMEM,
  MOVE_LIT_TO_MMEM,

  // COMPARISION INSTRUCTIONS
  JMP_NOT_EQ,

  // STACK MANIPULATION
  PSH_LIT,
  PSH_REG,
  POP,

  CAL_LIT,
  CAL_REG,

  RET,

  // HALT
  HLT,
};

#endif // INSTRUCTIONS_H_
