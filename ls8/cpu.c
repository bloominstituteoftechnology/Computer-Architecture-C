#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 1024;

/**
 * cpu_ram_read/cpu_ram_write
 */
unsigned char cpu_ram_read(struct cpu *cpu, int idx)
{
  return cpu->ram[idx];
}

void cpu_ram_write(struct cpu *cpu, int idx, unsigned char val)
{
  cpu->ram[idx] = val;
}

void push(struct cpu *cpu, unsigned char val)
{
  //Decrement the SP.
  //Copy the value in the given register to the address pointed to by SP.
  cpu->reg[SP]--;
  cpu->ram[cpu->reg[SP]] = val;
}

unsigned char pop(struct cpu *cpu)
{
  //Copy the value from the address pointed to by SP to the given register.
  //Increment SP.
  unsigned char val = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;
  return val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{

  int address = PROGRAM_START;

  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char line[1024];

  fp = fopen(filename, "r");

  while (fgets(line, sizeof line, fp) != NULL)
  {

    // Convert string to a number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);
    ;

    // Ignore lines from whicn no numbers were read
    if (endchar == line)
    {
      continue;
    }

    // Store in ram
    cpu->ram[address++] = byte;
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
    // TODO
    cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
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
    //PC
    int pc = cpu->PC;

    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, pc); //instruction register
    unsigned char operandA = cpu_ram_read(cpu, pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, pc + 2);

    printf("TRACE: %02X:  %02X  %02X  %02X\n", cpu->PC, IR, operandA, operandB);
    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    case (LDI):
      cpu->reg[operandA] = operandB;
      break;
    case (PRN):
      printf("\nValue at register %d: %d\n", operandA, cpu->reg[operandA]);
      break;
    case (MUL):
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case (PUSH):
      printf("Pushed value %d to stack\n", operandA);
      push(cpu, operandA);
      break;
    case (POP):
      printf("POPPING\n");
      break;

      // default:
      //   fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
      //   exit(3);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    cpu->PC += (IR >> 6) + 1;
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
  memset(cpu->reg, 0, sizeof cpu->reg);
  cpu->reg[SP] = EMPTY_STACK;
}
