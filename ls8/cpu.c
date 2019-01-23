#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DATA_LEN 6

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



unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}


void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
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
      break;

    case ALU_SUB:
      break;

    case ALU_DIV:
      break;

    case ALU_INC:
      break;

    case ALU_DEC:
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
    unsigned char operandA = NULL;  // initialize our operands for scope reach
    unsigned char operandB = NULL;    
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction

    if(num_operands == 2){
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff),
      operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
    } else if (num_operands == 1){
      operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    } else {
      // if no operands, set running to 0
      running = 0;
    }
    // 4. switch() over it to decide on a course of action.

    switch(instruction){

      case HLT:
        running = 0;
        break;

      case PRN:
      // print what is in the specified register
        break;

      case LDI:
        cpu->registers[operandA] = operandB;
        break;

      default:
        break;
    }
    // 5. Do whatever the instruction should do according to the spec.
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
}
