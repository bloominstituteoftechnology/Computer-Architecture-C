#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DATA_LEN 6

// unsigned char cpu_ram_read(struct cpu *cpu, int i)
// {
//   return cpu->ram[i];
// }

// void cpu_ram_write(struct cpu *cpu, int i, unsigned char value)
// {
//   cpu->ram[i] = value;
// }

void (*branchtable[256])(struct cpu *cpu, unsigned char, unsigned char) = {0};

void cpu_push(struct cpu *cpu, unsigned char val) {
  cpu->reg[SP]--;

  cpu->ram[cpu->reg[SP]] = val;
}

unsigned char cpu_pop(struct cpu *cpu) {
  unsigned char val = cpu->ram[cpu->reg[SP]];

  cpu->reg[SP]++;

  return val;
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

  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr = 0x00;

  if(fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  };

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr;

    unsigned char b;
    b = strtoul(line, NULL, 2);

    if (endptr == line) {
      continue;
    }
    // printf("%02X\n", b);

    cpu_ram_write(cpu, addr++, b);
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
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    }
    case ALU_ADD:
    {
      unsigned char sum = cpu->reg[regA] + cpu->reg[regB];
      cpu->reg[regA] = sum;
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
        print("%d\n", cpu->reg[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
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
