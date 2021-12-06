#ifndef CPU_H
#define CPU_H

#include "registers.h"

#define STACK_SIZE 64
#define MEMORY_SIZE 128
#define MMEMORY_SIZE 256
 
struct Cpu {
  bool halt;
  struct Stack *stack;
  int stack_frame_size;
  int registers[R_COUNT];
  int memory[MEMORY_SIZE];
  char *mapped_memory;
};

#endif // CPU_H
