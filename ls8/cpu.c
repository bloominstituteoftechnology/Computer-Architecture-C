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
    char instruction = cpu->pc;
    char operandA = cpu_read_ram(cpu, (instruction + 1));
    char operandB = cpu_read_ram(cpu, instruction + 2);

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  //memset(cpu->pc, 0, 1); -> is this needed?
  cpu->pc = 0;
  memset(cpu->registers, 0, 1);
  memset(cpu->ram, 0, 1);
}

/**
 * Read data from RAM
 */
char *cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  //To Do: implement function to read from ram
  //instruction = cpu->pc;

  return cpu->ram[index];

  //should not be void because it is returning something. should this return a pointer to a byte of memory?
}

/**
 * Write data to RAM
 */
void cpu_write_read(struct cpu *cpu, unsigned char index, unsigned char *value)
{
  //To Do: implement function to write to ram
  cpu->ram[index] = value;
}
