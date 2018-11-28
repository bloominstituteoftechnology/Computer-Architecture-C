#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 0

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp; // file pointer
  char line[256]; // storage for line of file
  int counter = 0; // way to track index in ram

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(1);
  }
    #if DEBUG
     printf("\n**********Lines from file:***********\n");
     #endif

    while (fgets(line, sizeof(line), fp) != NULL) {
      char *ptr;
      unsigned char byte = strtoul(line, &ptr, 2);
      if (ptr == line) {
        continue;
      }
      cpu->ram[counter++] = byte;
      
      #if DEBUG
      printf("Value of line: %s", line);
      #endif
    }
   
    #if DEBUG
    printf("\n*******RAM in Load*******\n");
    for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\n", i, cpu->ram[i]);
    }
    #endif
  // TODO: Replace this with something less hard-coded
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
    case ALU_ADD:
      reg[regA] += valB;
      break;
    case ALU_MUL:
      reg[regA] *= valB;
      break;
    // TODO: implement more ALU ops
  }
}
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
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        PC+=difference;
        break;
      case LDI:
        reg[operandA] = operandB;
        PC+=difference;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        PC+=difference;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        PC+=difference;
        break;
      case POP:
        reg[operandA] = cpu->ram[SP++];
        PC+=difference;
        break;
      case PUSH:
        cpu_ram_write(cpu, --SP, reg[operandA]);
        PC+=difference;
        break;
      case HLT:
        running = 0;
        break;
      case RET:
        PC = cpu->ram[SP++];
        break;
      case CALL:
        cpu_ram_write(cpu, --SP, PC+=difference);
        PC = cpu->reg[operandA];
        break;
      default:
        printf("Unknown instruction at %02x: %02x\n", cpu->PC, IR);
        exit(2);
    }

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
  #if DEBUG
    printf("\n********RAM in run********\n");
      for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\n", i, cpu->ram[i]);
    }
    printf("\n*******End of debugger********\n\n");
  #endif
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->reg = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
  cpu->reg[7] = 244;
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}