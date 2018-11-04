#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  unsigned char address = 0x00;
  unsigned char b;
  char line[1024];
  FILE *fp;

  fp = fopen("./examples/print8.ls8", "rb");
  while (fgets(line, sizeof line, fp) != NULL) {
    if ((line[0] == '\n') || (line[0] == '#')) continue;
    b = strtoul(line, NULL, 2);
    cpu->ram[address++] = b;
  }
  fclose(fp);  
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char current_inst, alu_op, sets_pc, current_reg, value;

  while (running) {
    // 1. Get the value of the current instruction (in address PC).

    current_inst = cpu->ram[cpu->pc];
    // alu_op = (current_inst & 0x20) >> 5;
    // sets_pc = (current_inst & 0x10) >> 4;
    // 2. switch() over it to decide on a course of action.
    switch(current_inst & 0x0f) {
    // 3. Do whatever the instruction should do according to the spec.
      case 0x02:
        cpu->reg[cpu->ram[cpu->pc + 1]] = cpu->ram[cpu->pc + 2];
        break;
      case 0x07:
        printf("%d\n", cpu->reg[cpu->ram[cpu->pc + 1]]);
        break;
      case 0x01:
        running = 0;
        break; 
    }
    // 4. Move the PC to the next instruction.
    cpu->pc = cpu->pc + (current_inst >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0x00;
  cpu->fl = 0x00;
  
  for (int i = 0; i < 7; i++) {
    cpu->reg[i] = 0x00;
  }
  cpu->reg[7] = 0xf4;
  cpu->reg[8] = '\0';

  for (int i = 0; i < 256; i++) {
    cpu->ram[i] = 0x00;
  }
  cpu->ram[256] = '\0';
}

void cpu_free(struct cpu *cpu) {
  free(cpu->reg);
  free(cpu->ram);
  free(cpu);
}
