#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ================== RAM functions ===================

/**  Read from the CPU's ram at the specifc location 
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

/**  Writes at index specified for the CPU's ram
 */
void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char num)
{
  cpu->ram[MAR] = num;
}

// ================== Instrcution Handlers ===================

// LDI function: Set the value of a register to an integer
void handle_LDI(struct cpu *cpu)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char value = cpu_ram_read(cpu, (cpu->pc + 2));
  cpu->registers[regA] = value;
};

// PRN function: Print numeric value stored in the given register
void handle_PRN(struct cpu *cpu)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  printf("%d\n", cpu->registers[regA]);
}

// HLT function: Halt the CPU (and exit the emulator)
void handle_HLT()
{
  exit(0);
}

// ================== CPU functions ===================

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv)
{
  FILE *ptr = fopen(argv, "r");
  char c[100];
  long ret;

  int address = 0;
  while (fgets(c, 100, ptr) != NULL)
  {
    if (c[0] == '1' || c[0] == '0')
    {
      ret = strtoul(c, NULL, 2);
      cpu_ram_write(cpu, address++, ret);
    }
  }

  fclose(ptr);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char regB = cpu_ram_read(cpu, (cpu->pc + 2));

  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
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
    unsigned char IR = cpu_ram_read(cpu, (cpu->pc));

    switch (IR)
    {
    case LDI:
      handle_LDI(cpu);
      break;

    case PRN:
      handle_PRN(cpu);
      break;

    case HLT:
      handle_HLT();
      break;

    case MUL:
      alu(cpu, ALU_MUL);
      break;

    default:
      printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->pc);
      exit(1);
    }

    int opNum = (11000000 & IR) >> 6;
    cpu->pc += (opNum + 1);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xF4;
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
