#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

// #define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *arg)
{
  int address = 0;
  FILE *f = fopen(arg, "r");
  char str[256];

  if (f == NULL)
  {
    printf("NULL");
    exit(1);
  }
  while (fgets(str, 256, f) != NULL)
  {
    int b = (int)strtol(str, NULL, 2);
    cpu->ram[address++] = b;
  }
  fclose(f);
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
 *  Reads from RAM
 */
char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Writes to RAM
 */
void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

/**
 * Perform LDI Instruction
 */
void ldi_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_read(cpu, cpu->pc + 2);
  cpu->reg[operand_a] = operand_b;
  cpu->pc += 3;
}

/**
 * Perform PRN Instruction
 */
void prn_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char print_value = cpu->reg[operand_a];

  printf("%u \n", print_value);

  cpu->pc += 2;
}

/**
 * Perform MUL Instruction
 */
void mul_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_read(cpu, cpu->pc + 2);
  cpu->reg[operand_a] = cpu->reg[operand_a] * cpu->reg[operand_b];
  cpu->pc += 3;
}

/**
 * Perform PUSH Instruction
 */
void push_instr(struct cpu *cpu)
{
  // Get the value at the next operand
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  // Decrement the available size of the stack
  cpu->reg[7]--;
  // In RAM, set the top stack value to the value in register[operand_a]
  cpu->ram[cpu->reg[7]] = cpu->reg[operand_a];
  // Increment the program counter
  cpu->pc += 2;
}

/**
 * Perform POP Instruction
 */
void pop_instr(struct cpu *cpu)
{
  // Get the value at the next operand
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  // Set the value at the specified register to the value at the top of the stack in RAM
  cpu->reg[operand_a] = cpu->ram[cpu->reg[7]];
  // Increase the size of the stack
  cpu->reg[7]++;
  // Increment the program counter
  cpu->pc += 2;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    unsigned char c_instr = cpu_ram_read(cpu, cpu->pc);
    switch (c_instr)
    {
    case LDI:
      ldi_instr(cpu);
      break;
    case PRN:
      prn_instr(cpu);
      break;
    case MUL:
      mul_instr(cpu);
      break;
    case PUSH:
      push_instr(cpu);
      break;
    case POP:
      pop_instr(cpu);
      break;
    case HLT:
      running = 0;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0;
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->reg, 0, 8 * sizeof(unsigned char));
}