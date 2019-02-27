#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

// cpu_ram_read
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  // address == MAR
  return cpu->ram[address];
}

// cpu_ram_write
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  // address == MAR value == MDR (Memory Data Register)
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fptr;
  char line[1024];
  unsigned char address = 0;
  fptr = fopen(file, "r");

  if (fptr == NULL)
  {
    fprintf(stderr, "error opening %s\n", file);
    exit(2);
  }

  while (fgets(line, sizeof(line), fptr) != NULL)
  {
    char *endptr;
    unsigned char binary_instruction;

    binary_instruction = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      continue;
    }

    cpu_ram_write(cpu, address++, binary_instruction);
  }

  fclose(fptr);
}

/**
 * ALU
 */

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char valA = cpu->registers[regA];
  unsigned char valB = cpu->registers[regB];

  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = valA * valB;
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
  unsigned char IR;
  unsigned char operandA;
  unsigned char operandB;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case POP:
      // Copy the value from the address pointed to by `SP` to the given register.
      cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
      //  increment SP
      cpu->registers[7]++;
      break;
    case PUSH:
      printf("SP: %d\n", cpu->registers[7]);
      // decrement the SP
      cpu->registers[7]--;
      // Copy the value in the given register to the address pointed to by`SP`.
      cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operandA]);
      break;
    case HLT:
      running = 0;
      break;
    default:
      printf("unexpected instruction 0x%02X at 0x%02X", IR, cpu->PC);
      exit(1);
      break;
    }

    // 6. Move the PC to the next instruction.
    // mask IR then shift it using the shift operator
    if ((0b11000000 & IR) >> 6 == 1)
    {
      cpu->PC += 2;
    }
    else if ((0b11000000 & IR) >> 6 == 2)
    {
      cpu->PC += 3;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // INIT REGISTERS
  memset(cpu->registers, 0, 7);
  cpu->registers[7] = 0xf4;
  // INIT PC
  cpu->PC = 0;
  // INIT RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
