#include "cpu.h"
#include <stdio.h>

#define DATA_LEN 6


// Read from memory
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}
// Write to memory
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value)
{
  cpu->ram[index] = value;
}



/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp;
  char data[1024];
  unsigned char address = 0;
  
  fp = fopen(filename, "r");

  while (fgets(data, sizeof data, fp) != NULL) {
    unsigned char code = strtoul(data, NULL, 2);
    if(data[0] == '\n' || data[0] == '#') {
      continue;
    }
    cpu->ram[address++] = code;
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
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char CI = cpu_ram_read(cpu, cpu->PC);

    unsigned char operand_a = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    unsigned char operand_b = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    switch(CI) {
      case LDI:
      printf("\nLDI: R%d: stored value is: %d\n\n", operand_a, operand_b);
      cpu->reg[operand_a] = operand_b;
      break;
    case PRN:
      printf("\nPRN: R%d: value is %d\n\n", operand_a, cpu->reg[operand_a]);
      break;
    case HLT:
      printf("\nHLT: Program halted\n\n");
      running = 0;
      exit(0);
    case MUL:
      alu(cpu, ALU_MUL, operand_a, operand_b);
      break;
    default:
      printf("\nThe instruction doesn't exist\n\n");
      exit(1);
    }
    
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));

  //Later on, you might do further initialization here, e.g. setting the initial value of the stack pointer.
}
