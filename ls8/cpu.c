#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset comes from here
#include "cpu.h"

/* Helper Function
*  for inhenced readability and debugging
*    1. data validation 
*    2. data encapsulation
*/

// reading from the ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// write to the ram
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");

  char line[256];

/*
  const int DATA_LEN = 6;
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
*/
  // TODO: Replace this with something less hard-coded
  int address = 0;

  if(fp == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  char *endchar;
  //Read all the lines and store them in RAM

  while(fgets(line, sizeof line, fp) != NULL)
  {
  unsigned char byte = strtol(line, &endchar, 2);

  // ignore empty lines
  if(endchar == line)
  {
    continue;
  }

  // storing the bytes in our RAM
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
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_ADD

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

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
    // 2. switch() over it to decide on a course of action.
    // what to do with the instructions?
    switch(IR) {
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->pc += 2;
        break;

      case HLT:
        running = 0;
        break;

      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->pc += 3;
        break;
      
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu->reg[operandA] *= cpu->reg[operandB];
        cpu->pc += 3;
        break;

      case ADD:
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu->reg[operandA] *= cpu->reg[operandB];
        cpu->pc += 3;
        break;

      
      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2); 
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.


    printf("Trace: %02x: %02x\n", cpu->pc, IR);


  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;

  // TODO: Zero registers and RAM

  // memset() is like fill() in JS
  memset(cpu->ram, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
}



// In cpu.c, add functions cpu_ram_read() and cpu_ram_write() that access the RAM inside the struct cpu.
// We'll make use of these helper function later.

// void cpu_ram_read(struct cpu *cpu) {

// }

// void cpu_ram_write(struct cpu *cpu) {

// }
