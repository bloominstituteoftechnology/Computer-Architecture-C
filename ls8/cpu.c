#include "cpu.h"
#include <stdio.h>
#include <string.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  const int DATA_LEN = 6;
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8  /* instruction */
    0b00000000, /* argument 1 */
    0b00001000, /* argument 2 */
    0b01000111, // PRN R0 /* instruction */
    0b00000000, /* argument 1 */
    0b00000001  // HLT /* instruction */
  };

  int address = 0;
  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu);


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

  // TODO
// 1. Get the value of the current instruction (in address PC).
// 2. switch() over it to decide on a course of action.
// 3. Do whatever the instruction should do according to the spec.
// 4. Move the PC to the next instruction.
void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  unsigned char PC = (int)cpu->PC;
  int running = 1; // True until we get a HLT instruction
  
  while (running) {
    unsigned char IR = cpu_ram_read(cpu, PC);
    int difference = (IR >> 6) + 1; // shifts the number 6 places to the right (leaving last two places)
    // since the number of operands can be found in the two high bits, add one for opcode to get to next instruction
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);

    switch(IR)
    {
      case LDI:
        reg[operandA] = operandB;
        PC+=difference;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        PC+=difference;
        break;
      case HLT:
        running = 0;
        break;
      default:
          break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;

  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}