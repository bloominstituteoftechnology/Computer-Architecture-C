#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare an array of pointers to functions and initialize them to NULL
void (*branchTable[256])(struct cpu *cpu, unsigned char, unsigned char) = {0};

// Return the value in memory
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// Write to memory with the given address / value
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char value = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;

  return value;
}

void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *file = fopen(filename, "r");
  char line[1024];

  int address = 0;

  // Writes the value of each line into memory
  while (fgets(line, sizeof line, file) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#')
    {
      continue;
    }

    cpu_ram_write(cpu, address++, strtol(line, NULL, 2));
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
    cpu->registers[regA] *= cpu->registers[regB];
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

    // Declare handler to be a pointer to a function
    void (*handler)(struct cpu * cpu, unsigned char, unsigned char);

    // Initialize handler to one of the handler functions in the branchTable array
    handler = branchTable[IR];

    // Checks if handler doesn't exist
    if (handler == NULL)
    {
      printf("Unknown instruction\n");
      return;
    }

    // Call the function given by the branchTable array
    handler(cpu, operandA, operandB);

    // Move on to the next instruction
    cpu->PC += (IR >> 6) + 1;
  }
}

// Handler Functions

void handle_LDI(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = operandB;
}

void handle_POP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = cpu_pop(cpu);
}

void handle_PUSH(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu_push(cpu, cpu->registers[operandA]);
}

void handle_MUL(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  alu(cpu, ALU_MUL, operandA, operandB);
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

  // Point SP to 0xF4 since stack is empty
  cpu->registers[SP] = 0xF4;

  // Initialize branchTable with the handler functions
  branchTable[LDI] = handle_LDI;
  branchTable[MUL] = handle_MUL;
  branchTable[PRN] = handle_PRN;
  branchTable[HLT] = handle_HLT;
  branchTable[POP] = handle_POP;
  branchTable[PUSH] = handle_PUSH;
}
