#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu -> ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu -> ram[address] = value;
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
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr = NULL;

    unsigned char b = strtoul(line, &endptr, 2);

    if (endptr == line) {
      // we got no numbers
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
    case ALU_MUL:
      // TODO
      cpu -> reg[regA] *= cpu -> reg[regB];
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
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu -> PC);

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, (cpu -> PC + 1) & 0xff);
    unsigned char operandB = cpu_ram_read(cpu, (cpu -> PC + 2) & 0xff);

    int add_to_pc = (IR >> 6) + 1;
    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);
    for(int i = 0; i < 8; i++) {
      printf(" %02X", cpu->reg[i]);
    }

    // 4. switch() over it to decide on a course of action.
    switch(IR) {
      case LDI:
        // 5. Do whatever the instruction should do according to the spec.
        cpu -> reg[operandA] = operandB;
        // 6. Move the PC to the next instruction.
        cpu -> PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu -> reg[operandA]);
        cpu -> PC += 2;
        break;
      case HLT:
        cpu -> PC += 1;
        return 0;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case POP:
        cpu -> reg[operandA & SP] = cpu_ram_read(cpu, cpu -> reg[SP]);
        cpu -> reg[SP]++;
        break;
      case PUSH:
        cpu -> reg[SP]--;
        cpu_ram_write(cpu, cpu -> reg[SP], cpu -> reg[operandA & SP]);
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA & SP];
        add_to_pc = 0;
        break;
      default:
        printf("unknown instruction %02x\n", IR);
    }
    cpu -> PC += add_to_pc;
    cpu -> PC &= 0xff;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu -> PC = 0;

  // initializes ram and reg from cpu.h
  memset(cpu -> ram, 0, sizeof(cpu -> ram));
  memset(cpu -> reg, 0, sizeof(cpu -> reg));
}
