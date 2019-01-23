#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6

/*
Add functions cpu_ram_read and cpu_ram_write to acces ram in struct cpu
*/

// if the read function reads from ram, it should return an unsigned char, based on how it's defined in struct cpu:
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {  // accept 2 arguments: a pointer to the cpu and an index in ram[] to access
	return cpu->ram[index];                                           // return the data held in ram at the specified index
}

// the write function doesn't need to return anything (I think) so it can be of type void:
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {  // accept 3 arguments: pointer to the cpu, an index to be written to, and a value to write
  	cpu->ram[index] = value;                                                     // set the value at the specified index equal to the specified value
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
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

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char PC = cpu->PC; // shorter variable to use below

  while (running) {
    
    // TODO
    // 1. Get the value of the current instruction (in address PC):
    unsigned char current_instruction = cpu_ram_read(cpu, PC);    // current_instruction == IR
    // 2. Figure out how many operands this next instruction requires:
    unsigned int num_operands = current_instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction:
    unsigned operandA = cpu_ram_read(cpu, (PC + 1));
	  unsigned operandB = cpu_ram_read(cpu, (PC + 2));

    // int shift = (num_operands) + 1;

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(current_instruction) {
      // HLT -- Halt the CPU and exit the emulator:
      case HLT:
        running = 0;
        break;

      // LDI -- Set a specified register to a specified integer value
      // in this case, set the next instruction to the instruction 2 steps ahead:
      case LDI:
        cpu->reg[operandA] = operandB;
		    // PC += shift;
		    break;

      // PRN -- Print numeric value stored in the given register:
      case PRN:
        printf("%d \n", cpu->reg[operandA]);
        // PC += shift;
		    break;

      default:
        break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0x00;

  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
