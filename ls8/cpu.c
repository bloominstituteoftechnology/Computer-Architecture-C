#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{

  // Original code that I used for print8:
  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8  /* instruction */
  //   0b00000000, /* argument 1 */
  //   0b00001000, /* argument 2 */
  //   0b01000111, // PRN R0 /* instruction */
  //   0b00000000, /* argument 1 */
  //   0b00000001  // HLT /* instruction */
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *fp; // file pointer
  char line[256]; // storage for line of file
  int counter = 0; // way to track index in ram

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(1);
  }
    while (fgets(line, sizeof(line), fp) != NULL) { // read line from file and store in line up to 256 bytes.
      char *ptr;
      unsigned char byte = strtoul(line, &ptr, 2);
      if (ptr == line) {
        continue;
      }
      cpu->ram[counter++] = byte; // converts string to unsigned long integer using base 2 and stores in ram
    }
    for (unsigned long i = 0; i < 256; i++) {
      printf("What is this? %u\n", cpu->ram[i]);
    }
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
    // case ALU_ADD:
    //   reg[regA = regA + regB;
    //   // TODO
    //   break;
    // case ALU_AND:
    //   regA = regA & regB;
    //   break;
    // case ALU_DEC:
    //   regA = regA - 1;
    //   break;
    // case ALU_SUB:
    //   regA = regA - regB;
    //   break;
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
  int running = 1; // True until we get a HLT instruction
  unsigned char SP = cpu->reg[7];
  
  while (running) {
    unsigned char IR = cpu_ram_read(cpu, PC);
    int difference = ((IR >> 6) & 0b11) + 1; // shifts the number 6 places to the right (leaving last two places)
    // since the number of operands can be found in the two high bits, add one for opcode to get to next instruction
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);

    switch(IR)
    {
      case LDI:
        reg[operandA] = operandB;
        // PC+=difference;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        // PC+=difference;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        // PC+= difference;
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
      for (unsigned long i = 0; i < 256; i++) {
      printf("What is this? %u\n", cpu->ram[i]);
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
  cpu->reg[7] = 244;
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
