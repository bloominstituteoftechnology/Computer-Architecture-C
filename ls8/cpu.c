#include <stdlib.h> // Exit is stored here.
#include <stdio.h> // Because you're using printf() without a prototype.
#include <string.h>
#include "cpu.h"

// Helper functions for efficiency -- to prevent repeating yourself.
// For better readability and detect bugs.
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
// Here, we are basically loading the .ls8 file to the RAM, which is essentially an array.
void cpu_load(struct cpu *cpu, char *file)
{

  // const unsigned int DATA_LEN = 6;
  // char data[6] = {
  //   // From print8.ls8
  // First line refers to the opcode; second line refers to the address in the register (R0), 
  // and the third refers to the binary value that's equivalent to 8 which we want to print.
  // The LDI binary value is actually a constant.
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < 6; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // Here, we are basically opening the .ls8 file in the RAM.
  // TODO: Replace this with something less hard-coded
  FILE *fp;
  fp = fopen(file, "r");

  int address = 0;

  char line[1000];

  while (fgets(line, sizeof(line), fp))
  {
    char *endptr;
    unsigned long int new_line;

    // The strtoul() function converts a string to an unsigned long integer.
    // Here, we're basically converting the binary strings to integer values to store in RAM.
    new_line = strtoul(line, &endptr, 2);
    cpu->ram[address++] = new_line;
  }

  fclose(fp);
}

/**
 * ALU
 */
// Basically, we are doing ALU operations here.
// ALU - Arithmetic Logic Unit, the fundamental building block of the CPU.
// "enum" is a user-defined data type. It's also a keyword.
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
      break;

    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
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
    // Instruction or opcode.
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);

    // Its arguments or operands.
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);

    // printf("TRACE: %02x: %02x\n", cpu->pc, IR);

    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB;
        // cpu->pc += 3;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        // cpu->pc += 2; 
        break;
        
      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      default:
        printf("Unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
    }

    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    cpu->pc += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // Loads the bytes in address 0.
  cpu->pc = 0;

  // TODO: Zero registers and RAM
}
