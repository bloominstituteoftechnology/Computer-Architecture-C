#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->reg[5]--;
  cpu->ram[cpu->reg[5]] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int address = 0;
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "File not found");
    exit(2);
  }
  while (fgets(line, sizeof line, fp) != NULL)
  {
    unsigned char j = strtol(line, NULL, 2);
    if (line[0] == '\n')
    {
      continue;
    }
    cpu->ram[address++] = j;
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
  int running = 1;  // True until we get a HLT instruction
  unsigned char IR; // Get the value of the current instruction, which is in the address of the pc
  while (running)
  {

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC); // PC is the current index in the instruction.
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    int add_to_pc = (IR >> 6) + 1;
    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB); // Trace output

    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB; // We take register number 0, and set it to the value 8.
      //cpu->PC += 3;                  // Move the pc to the next instruction.
      break;

    case MUL:
      cpu->reg[operandA] *= cpu->reg[operandB];
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;

    case HLT:
      running = 0;
      break;
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
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
  memset(cpu->ram, 0, sizeof cpu->ram); // Clear out the ram
  memset(cpu->reg, 0, sizeof cpu->reg); // Clear out the registers
}
