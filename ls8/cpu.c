#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, int i)
{
  return cpu->ram[i];
}

void cpu_ram_write(struct cpu *cpu, int i, unsigned char value)
{
  cpu->ram[i] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *fp;
  char line[1024];
  int address = ADDR_PROGRAM_ENTRY;

  if((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Can't open file %s\n", filename);
    exit(0);
  };

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);

    printf("%s", line);

    if (line[0] == '\n' || line[0] == '#') {
      printf("Ignoring this line");
      continue;
    }
    unsigned char b;
    b = strtoul(line, NULL, 2);

    printf("%02X\n", b);

    cpu->ram[address++] = byte;
  };

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
    {
      unsigned char product = regA * regB;
      cpu->reg[regA] = product;
      return 1;
      break;
    }
    case ALU_ADD:
    {
      unsigned char sum = regA + regB;
      cpu->reg[regA] = sum;
      return 1;
      break;
    }
    case LOAD_VALUE:
    {
      return cpu->reg[(int)regA];
      break;
    }
    case NOTHING:
    {
      break;
    }
    default:
      return 1;
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
    operandA = cpu_ram_read(cpu, cpu->PC+1);
    operandB = cpu_ram_read(cpu, cpu->PC+2);

    int add_to_pc = (IR >> 6) + 1;
    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);

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

    cpu->PC += add_to_pc;

    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    
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
