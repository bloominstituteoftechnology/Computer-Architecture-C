#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) 
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) 
{
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *file)
{
  FILE * f;
  f = fopen(file, "r");

  int address = 0;

  char line[256];

  while (fgets(line, sizeof(line), f)) {
    int i = 0;
    printf("%s", line);
    cpu->ram[address++] = line;
  }

  fclose(f);
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

  while (running) {
    // Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);

    switch(IR) {
      case LDI:
        cpu->registers[operandA] = operandB;
        break;

      case PRN:
        printf("print8: %d", cpu->registers[operandA]);
        break;

      case HLT:
        running = 0;
        break;

      default:
        printf("unknown instruction: %02x, %02x", cpu->pc, IR);
        exit(2);
    }

    // Move the PC to the next instruction.
    cpu->pc += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  // TODO: Zero registers and RAM
}
