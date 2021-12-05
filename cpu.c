#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "registers.h"
#include "instructions.h"

#define MEMORY_SIZE 128
#define STACK_SIZE 64

struct Cpu {
  bool halt;
  struct Stack *stack;
  int stack_frame_size;
  int registers[R_COUNT];
  int memory[MEMORY_SIZE];
};

struct Cpu *cpu_init()
{
  struct Cpu *cpu = (struct Cpu*) malloc(sizeof(struct Cpu));
  cpu->stack = createStack(STACK_SIZE);

  cpu->halt = false;

  for(int i = 0; i < MEMORY_SIZE; cpu->memory[i++] = 0); 
  for(int i = 0; i < R_COUNT; cpu->registers[i++] = 0); 

  return cpu;
}

int get_register(struct Cpu *cpu, int register_name)
{
  return cpu->registers[register_name];
}

void set_register(struct Cpu *cpu, int register_name, int value)
{
  cpu->registers[register_name] = value;
}

int fetch(struct Cpu *cpu)
{
  int instruction_address = get_register(cpu, R_IP);
  int instruction = cpu->memory[instruction_address];
  set_register(cpu, R_IP, get_register(cpu, R_IP) + 1);

  return instruction;
}

void pop_state(struct Cpu *cpu)
{
  int frame_pointer_address = get_register(cpu, R_FP);
  set_register(cpu, R_SP, frame_pointer_address);

  cpu->stack_frame_size = pop(cpu->stack);
  int _stack_frame_size = cpu->stack_frame_size;
  set_register(cpu, R_IP, pop(cpu->stack));
  set_register(cpu, R_R7, pop(cpu->stack));
  set_register(cpu, R_R6, pop(cpu->stack));
  set_register(cpu, R_R5, pop(cpu->stack));
  set_register(cpu, R_R4, pop(cpu->stack));
  set_register(cpu, R_R3, pop(cpu->stack));
  set_register(cpu, R_R2, pop(cpu->stack));
  set_register(cpu, R_R1, pop(cpu->stack));
  set_register(cpu, R_R0, pop(cpu->stack));

  int num_of_args = pop(cpu->stack);
  for (int i = 0; i < num_of_args; i++) 
  {
    pop(cpu->stack);
  }
  
  set_register(cpu, R_FP, frame_pointer_address + _stack_frame_size);
}

void push_state(struct Cpu *cpu)
{
  push(cpu->stack, get_register(cpu, R_R0));
  push(cpu->stack, get_register(cpu, R_R1));
  push(cpu->stack, get_register(cpu, R_R2));
  push(cpu->stack, get_register(cpu, R_R3));
  push(cpu->stack, get_register(cpu, R_R4));
  push(cpu->stack, get_register(cpu, R_R5));
  push(cpu->stack, get_register(cpu, R_R6));
  push(cpu->stack, get_register(cpu, R_R7));
  push(cpu->stack, get_register(cpu, R_IP));
  push(cpu->stack, cpu->stack_frame_size + 1);

  set_register(cpu, R_FP, get_register(cpu, R_SP));
  cpu->stack_frame_size = 0;
}

void cpu_free(struct Cpu *cpu)
{
  free(cpu->stack);
  free(cpu);
}

void execute(struct Cpu *cpu, int instruction)
{
  switch (instruction) {
    case MOVE_LIT_TO_REG:
    {
      int value = fetch(cpu);
      int register_name = fetch(cpu);
      set_register(cpu, register_name, value);
      break;
    }
    case MOVE_REG_TO_REG:
    {
      int r1 = fetch(cpu);
      int r2 = fetch(cpu);

      int r1_value = get_register(cpu, r1);

      set_register(cpu, r2, r1_value);
      break;
    }

    case MOVE_REG_TO_MEM:
    {
      int register_name = fetch(cpu);
      int memory_address = fetch(cpu);

      int register_value = get_register(cpu, register_name);

      cpu->memory[memory_address] = register_value;

      break;
    }

    case MOVE_MEM_TO_REG:
    {
      int memory_address = fetch(cpu);
      int register_name = fetch(cpu);

      set_register(cpu, register_name, cpu->memory[memory_address]);

      break;
    }

    case ADD_REG_TO_REG:
    {
      int r1 = fetch(cpu);
      int r2 = fetch(cpu);

      int r1_value = get_register(cpu, r1);
      int r2_value = get_register(cpu, r2);

      set_register(cpu, R_ACC, r1_value + r2_value);
      break;
    }

    case JMP_NOT_EQ:
    {
      int value = fetch(cpu);
      int address = fetch(cpu);

      if (get_register(cpu, R_ACC) != value) 
      {
        cpu->registers[R_IP] = address;
      }

      break;
    }

    case PSH_LIT:
    {
      int value = fetch(cpu);
      push(cpu->stack, value);
      cpu->stack_frame_size++;
      break;
    }

    case PSH_REG:
    {
      int reg = fetch(cpu);
      push(cpu->stack, get_register(cpu, reg));
      cpu->stack_frame_size++;
      break;
    }

    case POP:
    {
      int register_name = fetch(cpu);
      set_register(cpu, register_name, pop(cpu->stack));
      cpu->stack_frame_size--;
      break;
    }

    case CAL_LIT:
    {
      int address = fetch(cpu);
      push_state(cpu);
      set_register(cpu, R_IP, address);
      break;
    }

    case CAL_REG:
    {
      int register_name = fetch(cpu);
      int address = get_register(cpu, register_name);
      push_state(cpu);
      set_register(cpu, R_IP, address);
      break;
    }

    case RET:
    {
      pop_state(cpu);
      break;
    }

    case HLT:
    { 
      cpu->halt = true;
    }
  }
}

void step(struct Cpu *cpu) {
  int instruction = fetch(cpu);
  execute(cpu, instruction);
}

void run(struct Cpu *cpu)
{
  step(cpu);
  if(cpu->halt) return;
  run(cpu);
}

int main()
{
  
  struct Cpu *cpu = cpu_init();

  int i = 0;

  cpu->memory[i++] = MOVE_LIT_TO_REG; 
  cpu->memory[i++] = 0xAB;
  cpu->memory[i++] = R_R1;

  cpu->memory[i++] = MOVE_REG_TO_REG;
  cpu->memory[i++] = R_R1;
  cpu->memory[i++] = R_R2;

  cpu->memory[i++] = ADD_REG_TO_REG;
  cpu->memory[i++] = R_R1;
  cpu->memory[i++] = R_R2;

  cpu->memory[i++] = MOVE_REG_TO_MEM;
  cpu->memory[i++] = R_ACC;
  cpu->memory[i++] = 5;
  
  cpu->memory[i++] = PSH_LIT;
  cpu->memory[i++] = 3;

  cpu->memory[i++] = PSH_LIT;
  cpu->memory[i++] = 3;

  cpu->memory[i++] = POP;
  cpu->memory[i++] = R_R2;

  cpu->memory[i++] = HLT;

  run(cpu);

  int reg = get_register(cpu, R_R2);

  printf("%i\n", reg);
  printf("%i\n", cpu->memory[5]);

  printf("stack: %i\n", peek(cpu->stack));
  printf("stack top is : %i\n", cpu->stack->top);

  cpu_free(cpu);

  return 0;
}
