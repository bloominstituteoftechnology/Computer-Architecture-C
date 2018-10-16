#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

void (*branchTable[256])(struct cpu *cpu, unsigned char, unsigned char) = {0};

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
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *file = fopen(filename, "r");
  char line[1024];

  int address = 0;

  while (fgets(line, sizeof line, file) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#')
    {
      continue;
    }

    cpu->ram[address++] = strtol(line, NULL, 2);
  }

  fclose(file);
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

  while (cpu->running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    void (*handler)(struct cpu * cpu, unsigned char, unsigned char);

    handler = branchTable[IR];

    if (handler == NULL)
    {
      printf("Unknown instruction\n");
      return;
    }

    handler(cpu, operandA, operandB);

    cpu->PC += (IR >> 6) + 1;
  }
}

void handle_LDI(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = operandB;
}

void handle_MUL(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = cpu->registers[operandA] * cpu->registers[operandB];
}

void handle_PRN(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  printf("%d\n", cpu->registers[operandA]);
}

void handle_HLT(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->running = 0;
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->running = 1;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->ram, 0, sizeof cpu->registers);

  branchTable[LDI] = handle_LDI;
  branchTable[MUL] = handle_MUL;
  branchTable[PRN] = handle_PRN;
  branchTable[HLT] = handle_HLT;
}
