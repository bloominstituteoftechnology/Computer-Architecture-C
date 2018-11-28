#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

#define SP 5

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *argv[])
{

  FILE *fp;
  char data[1024];
  unsigned char address = 0;
  fp = fopen(argv[1], "r");

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
    unsigned char curr = cpu_ram_read(cpu, cpu->PC);
    unsigned char param1 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char param2 = cpu_ram_read(cpu, cpu->PC + 2);

    // 2. switch() over it to decide on a course of action.
    switch (curr)
    {
    case LDI:
      cpu->registers[param1] = param2;
      break;
    case PRN:
      printf("%d \n", cpu->registers[param1]);
      break;
    // case MUL:
    //   alu(cpu, ALU_MUL, param1, param2);
    //   break;
    case HLT:
      running = 0;
      break;
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    cpu->PC += (curr >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
}
