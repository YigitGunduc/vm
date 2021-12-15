#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

// local includes
#include "stack.h"
#include "registers.h"
#include "instructions.h"
#include "mapped_memory.h"
#include "cpu.h"

struct Cpu *cpu_init()
{
  struct Cpu *cpu = (struct Cpu*) malloc(sizeof(struct Cpu));
  cpu->stack = createStack(STACK_SIZE);
  cpu->halt = false;

  cpu->mapped_memory = map_memory_for_write("./out.txt", MMEMORY_SIZE);

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

    case MOVE_LIT_TO_MEM:
      {
        int literal = fetch(cpu);
        int memory_address = fetch(cpu);

        cpu->mapped_memory[memory_address] = literal;

        break;
      }

    case MOVE_REG_TO_MMEM:
      {
        int register_name = fetch(cpu);
        int memory_address = fetch(cpu);

        int register_value = get_register(cpu, register_name);

        cpu->mapped_memory[memory_address] = (char) register_value;

        break;
      }

    case MOVE_MMEM_TO_REG:
      {
        int memory_address = fetch(cpu);
        int register_name = fetch(cpu);


        set_register(cpu, register_name, cpu->mapped_memory[memory_address]);

        break;
      }

    case MOVE_LIT_TO_MMEM:
      {
        int literal = fetch(cpu);
        int memory_address = fetch(cpu);

        cpu->mapped_memory[memory_address] = (char) literal;
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

    case ADD_LIT_TO_REG:
      {
        int literal = fetch(cpu);
        int register_name = fetch(cpu);

        int register_value = get_register(cpu, register_name);

        set_register(cpu, R_ACC, register_value + literal);
        break;
      }

    case INC_ACC:
      {
        set_register(cpu, R_ACC, R_ACC + 1);
        break;
      }

    case DEC_ACC:
      {
        set_register(cpu, R_ACC, get_register(cpu, R_ACC) - 1);
        break;
      }

    case SUB_LIT_TO_REG:
      {
        int literal = fetch(cpu);
        int register_name = fetch(cpu);

        int register_value = get_register(cpu, register_name);

        set_register(cpu, R_ACC, register_value - literal);
        break;
      }

    case SUB_REG_TO_REG:
      {
        int r1 = fetch(cpu);
        int r2 = fetch(cpu);

        int r1_value = get_register(cpu, r1);
        int r2_value = get_register(cpu, r2);

        set_register(cpu, R_ACC, r1_value - r2_value);
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
        break;
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

