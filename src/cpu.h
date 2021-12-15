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


struct Cpu *cpu_init();
int get_register(struct Cpu *cpu, int register_name);
void set_register(struct Cpu *cpu, int register_name, int value);
int fetch(struct Cpu *cpu);
void pop_state(struct Cpu *cpu);
void push_state(struct Cpu *cpu);
void cpu_free(struct Cpu *cpu);
void execute(struct Cpu *cpu, int instruction);
void step(struct Cpu *cpu);
void run(struct Cpu *cpu);

#endif // CPU_H
