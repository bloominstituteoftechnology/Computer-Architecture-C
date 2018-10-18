#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp; 
  char line[256]; 
  int counter = 0; 

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(1);
  }
    while (fgets(line, sizeof(line), fp) != NULL) { 
      char *ptr;
      unsigned char byte = strtoul(line, &ptr, 2);
      if (ptr == line) {
        continue;
      }
      cpu->ram[counter++] = byte; 
    }
}

unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}


/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valB = reg[regB];
  switch (op) {
    case ALU_MUL:
      reg[regA] *= valB;
      break;
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  unsigned char PC = cpu->PC;
  int running = 1; 
  unsigned char SP = cpu->reg[7];
  
  while (running) {
    unsigned char IR = cpu_ram_read(cpu, PC);
    int difference = ((IR >> 6) & 0b11) + 1; 
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);

    switch(IR)
    {
      case LDI:
        reg[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case POP:
        reg[operandA] = cpu->ram[SP++];
        break;
      case PUSH:
        cpu_ram_write(cpu, --SP, reg[operandA]);
        break;
      case HLT:
        running = 0;
        break;
      default:
        printf("Unknown instruction at %02x: %02x\n", cpu->PC, IR);
        exit(2);
    }
    PC+=difference;
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->reg = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}