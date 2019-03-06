#include "cpu.h"

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
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
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

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    //IR gives you the current instuction
    //reading from the bitwise right shift will give you the number of operands:
    unsigned char operandA = 0;
    unsigned char operandB = 0;
    unsigned int numOp = IR >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    if (numOp == 2)
    {
      unsigned char operandA = cpu_ram_read(cpu, (cpu->pc + 1) & 0xff);
      unsigned char operandB = cpu_ram_read(cpu, (cpu->pc + 2) & 0xff);
    }
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:
      // change the value of reg @ operanA to operandB;
      cpu->reg[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;
    case HLT:
      running = 0;
      break;
    default:
      printf("Error");
      break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

// ram functions
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  for (int i = 0; i <= 6; i++)
  {
    cpu->reg[i] = 0;
  }

  cpu->reg[7] = 0;
  cpu->pc = 0;

  // set everything in ram to 0
  memset(cpu->ram, 0, sizeof cpu->ram);
}