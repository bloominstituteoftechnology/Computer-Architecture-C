#include "cpu.h"

#define DATA_LEN 6
#define LDI 0b10000010
#define NOP 0b00000000
#define PRN 0b01000111
#define HLT 0b00000001
#define STR 0b00001000 // store the next value to reg

// read what is in the ram
int cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

// write to the ram
void cpu_ram_write(struct cpu *cpu, int index)
{
  char *space = malloc(cpu->ram, sizeof(cpu->ram));
  for (int i = 0; i < sizeof(cpu->ram); i++)
  {
    if (cpu->ram[i] == '\0')
    {
      cpu->ram[i] = space;
    }
    else
    {
      printf("No space available");
    }
  }
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
      // From print8.ls8
      //LDI load immediate, store a value in reg or set this reg to this value
      LDI, // LDI R0,8
      NOP,
      STR, //README says this is value of 8
      8,   // make STR store this to registry
      //PRN prints the numberic value stored in a reg
      PRN, // PRN R0
      NOP,
      // halt the cpu and exit the emulator
      HLT // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    if (data[i] == STR)
    {
      cpu->registers[address] = data[i + 1];
    }
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB) // what are these?
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
}
