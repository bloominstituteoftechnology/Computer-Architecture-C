#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar) 
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr) 
{
  cpu->ram[mar] = mdr;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr = 0x00;

  if (fp == NULL) {
    fprintf(stderr, "error opening file %\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr;
    unsigned char b = strtoul(line, &endptr, 2);

    // Input "1000010 # Comment"
    

    cpu_ram_write(cpu, addr++, b);
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
  unsigned char IR, operandA, operandB; 

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);

    int add_to_pc = (IR >> 6) + 1;

    // traces operands
    print("TRACE: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);

    // 2. switch() over it to decide on a course of action.
    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case PRN:
        print("%d\n", cpu->reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;
    }

    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
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
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
}
