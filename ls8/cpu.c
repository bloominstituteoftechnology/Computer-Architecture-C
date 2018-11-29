#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define SP 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  char buffer[1024];
  unsigned char address = 0;
  fp = fopen(argv[1], "r");

  if (!fp)
  {
    printf("Error: File could not be opened \n");
    exit(1);
  }

  while (fgets(buffer, sizeof buffer, fp) != NULL)
  {
    cpu->ram[address++] = strtol(buffer, NULL, 2);
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] *= cpu->registers[regB];
    break;

    // TODO: implement more ALU ops
  }
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = val;
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char ret = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;
  return ret;
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    unsigned char curr = cpu_ram_read(cpu, cpu->PC);
    unsigned char value1 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char value2 = cpu_ram_read(cpu, cpu->PC + 2);

    switch (curr)
    {
    case LDI:
      cpu->registers[value1] = value2;
      break;
    case PRN:
      printf("%d \n", cpu->registers[value1]);
      break;
    case HLT:
      running = 0;
      break;
    case MUL:
      alu(cpu, ALU_MUL, value1, value2);
      break;
    case PUSH:
      cpu_push(cpu, cpu->registers[value1]);
      break;
    case POP:
      cpu->registers[value1] = cpu_pop(cpu);
      break;
    }

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
