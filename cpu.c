#include <stdio.h>
#include "registers.h"

#define MEMORY_SIZE 128

struct Cpu {
  int memory[MEMORY_SIZE];
  int reg[R_COUNT];
};

int get_register(struct Cpu *cpu, int register_name)
{
  return cpu->reg[register_name];
}

void set_register(struct Cpu *cpu, int register_name, int value)
{
  cpu->reg[register_name] = value;
}

int fetch(struct Cpu *cpu)
{
  int instruction_address = get_register(cpu, R_PC);
  int instruction = cpu->memory[instruction_address];
  set_register(cpu, R_PC, get_register(cpu, R_PC) + 1);

  return instruction;
}

void execute(struct Cpu *cpu, int instruction)
{
  if (0x10 == instruction)
  {
    int value = fetch(cpu);
    set_register(cpu, R_R1, value);
  }

  if (0x11 == instruction)
  {
    int value = fetch(cpu);
    set_register(cpu, R_R2, value);
  }

  if (0x12 == instruction)
  {
    int r1 = fetch(cpu);
    int r2 = fetch(cpu);
    int r1_value = get_register(cpu, r1);
    int r2_value = get_register(cpu, r2);
    set_register(cpu, R_ACC, r1_value + r2_value);
  }
}

int step(struct Cpu *cpu) {
  int instruction = fetch(cpu);
  return instruction;
}

int main()
{
  struct Cpu cpu;

  cpu.memory[0] = 0x10;
  cpu.memory[1] = 0xAB;

  cpu.memory[2] = 0x11;
  cpu.memory[3] = 0x34;

  cpu.memory[4] = 0x12;
  cpu.memory[5] = R_R1;
  cpu.memory[6] = R_R2;

  set_register(&cpu, R_PC, 0);

  execute(&cpu, step(&cpu));
  execute(&cpu, step(&cpu));
  execute(&cpu, step(&cpu));

  int reg = get_register(&cpu, R_ACC);

  printf("%i\n", reg);

  return 0;
}
