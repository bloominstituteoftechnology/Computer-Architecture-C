#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)  // helper function; return the address of the cpu
{
  return cpu->ram[address]; // the cpu at this address will return the bytes that's there
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)  // helper function; return the address of the cpu and value
{
  cpu->ram[address] = value; 
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    unsigned char IR = cpu_ram_read(cpu, cpu->pc); // read from RAM at the PC; this is the instruction/ opcode

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1); // reading at the next address of whatever the pc points at
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2); // reading the next byte and store in operand B

    printf("TRACE: %02x: %02x\n", cpu->pc, IR);

    switch (IR) { // switch on the IR 

      case LDI;   // LDI instruction
        cpu->reg[operandA] = operandB; // operandA tells us the register number we need to store the value in; operandB is the value we need to store 
        cpu->pc += 3; // increment the pc; LDI is 3 bytes
        break;
       
      default;  // default case 
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR); // if we get instruction doesn't know, this will tell us where and what it is
        exit(2);  // exit 
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;  // load instructions at address 0

  // TODO: Zero registers and RAM
}
