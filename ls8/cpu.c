#include "cpu.h"
#include <stdio.h>
#include <string.h>
#define DATA_LEN 6

// Implement CPU_RAM_READ & CPU_RAM_WRITE

// 1. Take in the cpu || an index to write into || and a value to write into it
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char val) {
    // 2. Set given index to the value given
    cpu->ram[index] = val;
}

// 1. Take in a cpu || index they want to read
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index]; // cpu->ram[cpu->pc]
}

/*

  index:
  1. 0b10000010
  2. 
  3.
  ...
  256.

*/

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = { // --> RAM ARRAY 
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
  unsigned char operand_a;
  unsigned char operand_b;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    unsigned int num_of_operands = instruction >> 6; // --> instruction >> 6
    // --> 0000011 --> 00000011
    // 3. Get the appropriate value(s) of the operands following this instruction          
      // --> 5. Do whatever the instruction should do according to the spec.
    if (num_of_operands == 2) {
      operand_a = cpu_ram_read( cpu, (cpu->PC + 1) & 0xff );
      operand_b = cpu_ram_read( cpu, (cpu->PC + 2) & 0xff );
      printf("2 operands found\n");
    } else if (num_of_operands == 1) {
      operand_a = cpu_ram_read( cpu, (cpu->PC + 1) & 0xff );
      printf("1 operand found\n");
    }

    // --> 4. switch() over it to decide on a course of action.
    switch(instruction) {
      case HLT:
        running = 0;
        break;
      
      case LDI:        
        cpu->registers[operand_a] = operand_b;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operand_a]);
        break;
      
      default:
        break;
    } // --> switch()

    // 6. Move the PC to the next instruction.
    cpu->PC += num_of_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers  
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
