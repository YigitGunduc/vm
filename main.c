#include <stdio.h>
#include <stdbool.h>

#include "src/cpu.h" 
#include "src/registers.h"
#include "src/instructions.h"

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

  cpu->memory[i++] = POP;
  cpu->memory[i++] = R_R2;

  cpu->memory[i++] = HLT;

  run(cpu);

  int reg = get_register(cpu, R_R2);

  printf("value of r2 is %i\n", reg);

  cpu_free(cpu);

  return 0;
}
