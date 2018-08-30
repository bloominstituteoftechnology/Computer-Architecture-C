#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

// reading from the ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address]; // the cpu will  return address of the ram
}

// write to the ram
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");

  char line[256]; // create a buffer for each line in memory

  // TODO: Replace this with something less hard-coded

  int address = 0;

      if (fp == NULL)
  {
    fprintf(stderr, "Can not open file %s\n", filename);
    exit(2);
  }
  // printf("reached here\n");
  // exit(3);

  char *endchar;

  // Read all the lines and store them in RAM
  while(fgets(line, sizeof line, fp) != NULL){
    unsigned char byte = strtol(line, &endchar, 2);

    // ignore empty lines
    if (endchar == line)
    {
      continue;
    } 

    // Storing bytes in our ram 
    cpu_ram_write(cpu, address++, byte);
  }

  
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    case ALU_ADD:
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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    // 2. switch() over it to decide on a course of action.

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);


    switch(IR) {

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->pc += 2; 
        break;

      case LDI:
        printf("We are about to load\n");
        cpu->reg[operandA] = operandB;
        cpu->pc += 3;
        break;

      case ADD:
        printf("We are about to load\n");
        cpu->reg[operandA] += cpu->reg[operandB];
        cpu->pc += 3;
        break;

      case MUT:
        cpu->reg[operandA] *= cpu->reg[operandB];
        cpu->pc += 3; 
        break;

      case HLT: 
        printf("currently inside the halt method\n");
        running = 0; // turns off the loop
        break;

        // 3. Do whatever the instruction should do according to the spec.
        // 4. Move the PC to the next instruction.

      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
    }
    
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0; // initialize/ load instructions at address 0

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg); // grab the registers; we fill in the array with zeros
  memset(cpu->ram, 0, sizeof cpu->ram); // grab the ram; we fill in the array with zeros
}

