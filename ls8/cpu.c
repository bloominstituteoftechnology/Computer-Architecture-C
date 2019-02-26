#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{

  int address = 0;
  FILE *fp;
  char line[1024];

  if ((fp = fopen(argv[1], "r")) == NULL)
  {
    fprintf(stderr, "Cannot open print8.ls8\n");
    exit(1);
  }
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *ptr;
    unsigned char ret = strtol(line, &ptr, 2);

    if (ptr == line)
    {
      continue;
    }

    cpu->ram[address++] = ret;
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
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    switch (instruction)
    {
    case LDI:
      // 5. Do whatever the instruction should do according to the spec.
      cpu->reg[operandA] = operandB;
      // 6. Move the PC to the next instruction.
      cpu->PC += 3;
      break;
    case PRN:
      printf("%u\n", operandA);
      cpu->PC += 2;
      break;
    case HLT:
      cpu->PC += 1;
      return 0;
    case MUL:
    alu(cpu, ALU_MUL, operandA, operandB);
    cpu->PC += 3;
    break;
    }
    cpu->PC += 1 + (int)(instruction >> 6);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  //void *memset(void *ptr, int x, size_t n);
  //initalize both ram and reg from cpu.h
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
}
