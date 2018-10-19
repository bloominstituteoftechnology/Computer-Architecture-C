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
    fprintf(stderr, "Error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr;

    unsigned char b = strtoul(line, &endptr, 2);

    if (endptr == line) {
      continue;
    }

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
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;

    case ALU_SUB:
      cpu->reg[regA] -= cpu->reg[regB];
      break;

    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    case ALU_DIV:
      cpu->reg[regA] /= cpu->reg[regB];
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
  unsigned char SP = cpu->reg[7];

  while (running) {

    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, cpu->PC+1);
    operandB = cpu_ram_read(cpu, cpu->PC+2);

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    for(int i = 0; i < 8; i++) {
      printf(" %02X", cpu->reg[i]);
    }

    printf("\n");

    // 2. switch() over it to decide on a course of action.
    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case DIV:
        alu(cpu, ALU_DIV, operandA, operandB);
        break;
      
      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, SP++);
        break;

      case PUSH:
        cpu_ram_write(cpu, --SP, cpu->reg[operandA]);
        break;

      case CMP:
        if (cpu->reg[operandA] == cpu->reg[operandB])
          cpu->FL = 1;
        else
          cpu->FL = 0;
        break;

      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;

      case JEQ:
        if (cpu->FL)
          cpu->PC = cpu->reg[operandA];
        else
          cpu->PC =+ 2;
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
  // Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = FLAG;

  // Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
}
