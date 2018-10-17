#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

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
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(file, "r");

  while (fgets(line, sizeof line, fp) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#') continue;

    unsigned char b;
    b = strtoul(line, NULL, 2);
    cpu->ram[address++] = b;
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] *= cpu->reg[regB];
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
  unsigned char IR, opA, opB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    opA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    opB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, opA, opB);

    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case LDI:
        cpu->reg[opA] = opB;
        break;

      case MUL:
        alu(cpu, ALU_MUL, opA, opB);
        break;

      case PRN:
        printf("%d\n", cpu->reg[opA]);
        break;

      case HLT:
        running = 0;
        break;
    }

    // 3. Do whatever the instruction should do according to the spec.
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

  // cpu->reg[7] = 0xF4;
}
