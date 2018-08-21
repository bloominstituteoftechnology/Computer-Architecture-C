#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->RAM[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->RAM[address] = value;
}

void cpu_load(struct cpu *cpu, char *argv[])
{
  // file pointer
  FILE *fp;
  int address = 0;
  char read[25];
  unsigned long instruction;

  fp = fopen(argv[1], "r");
  if(fp == NULL) {
    fprintf(stderr, "Filepointer goofed\n");
    exit(1);
  }

  // loop until fgets is at the end of the file
  while (fgets(read, sizeof(read), fp)) {
    // cast line to binary until instruction is captured
    instruction = strtoul(read, NULL, 2);
    // write instruction to RAM @address
    cpu_ram_write(cpu, address++, instruction);
    }
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
    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
      printf("ADD instruciton for ALU provided\n"); 
      break;
    default:
      printf("No instruciton for ALU provided\n"); 
      break;
  }
}

void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running) {
    // Read Instructions from RAM
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);

    // Read additional operands 
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    switch(IR){
      case LDI:
        // Set register to int
        cpu->reg[operandA] = operandB;
        break;
      case PRN:
        // print register
        printf("%d\n", cpu->reg[operandA]);
        break;
      case HLT:
        // HALT
        running = 0;
        break;
      case MUL:
        // Call ALU to execute MUL instruction
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
    }
    // add to the PC according to the executed instruction
    // using >> bitwise shifting of the binary instruction
    cpu->pc += (IR >> 6) + 1;
  }
}

void cpu_init(struct cpu *cpu)
{
  // Zeroing out the pc, registers and ram
  cpu->pc = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->RAM, 0, sizeof(cpu->RAM));
}
