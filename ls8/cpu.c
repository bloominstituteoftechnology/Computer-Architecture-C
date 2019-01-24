#include "cpu.h"
#include "string.h"
#include "stdio.h"
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
  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    perror("Could not open");
  }

  char string[256];
  unsigned char address = 0;
  while (fgets(string, sizeof(string), fp) != NULL)
  {

    unsigned long int data = strtoul(string, NULL, 2);
    cpu->ram[address] = data;
    address++;
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
    cpu->reg[regA] *= cpu->reg[regB];
    break;

  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;

  case ALU_INC:
    // TODO
    break;

  case ALU_DEC:
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
  unsigned char instruction;
  unsigned int num_operands = instruction >> 6;

  unsigned char operandA = 0;
  unsigned char operandB = 0;
  if (num_operands == 1)
  {
    operandA = cpu->ram[cpu->PC + 1];
  }
  else if (num_operands == 2)
  {
    operandB = cpu->ram[cpu->PC + 2];
  }
  else
  {
    return 0;
  }

  while (running)
  {

    switch (instruction)
    {
    case HLT:
      running = 0;
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;

    case LDI:
      cpu->reg[operandA] = operandB;
      break;

    default:
      break;
    }

    cpu->PC += num_operands + 1;
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
  cpu = malloc(sizeof(cpu));
  cpu->PC = 0;
  memset(cpu->reg, 0, 8);
  memset(cpu->ram, 0, 256);
  // TODO: Initialize the PC and other special registers
}
