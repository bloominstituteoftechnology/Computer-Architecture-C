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

#if 0
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
#endif
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  (void)cpu;  // hack to suppress compiler warnings
  (void)regA;
  (void)regB;

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

    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        // cpu->PC += 3;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        // cpu->PC += 2;
        break;

      case HLT:
        running = 0;
        // cpu->PC += 1;
        break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
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

  // Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
}