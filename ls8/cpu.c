#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char byte)
{
  cpu->ram[address] = byte;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *directory)
{
  FILE *file = fopen(directory, "r");

  if (file == NULL)
  {
    printf("invalid file\n");
    exit(2);
  }

  char line[8192];
  int address = 0;

  while (fgets(line, sizeof line, file) != NULL)
  {
    // printf("%s", line);
    char *endptr;
    unsigned char byte = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, byte);
  }

  fclose(file);

  // FILE *file = fopen(directory, "r");
  // char *line = NULL;
  // size_t len = 0;
  // ssize_t read;

  // if (directory == NULL)
  //   printf("program not found");
  // return;

  // while ((read = getline(&line, &len, file)) != -1)
  // {
  //   printf("Retrieved line of length %zu :\n", read);
  //   printf("%s", line);
  // }

  // free(line);
  // fclose(file);
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
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA;
    unsigned char operandB;

    int nextInstruction = 1;

    if (IR & 0x80)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
      operandB = cpu_ram_read(cpu, cpu->PC + 2);

      nextInstruction = 3;
    }
    else if (IR & 0x40)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);

      nextInstruction = 2;
    }

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case HLT:
      running = 0;
      return;

    case LDI:
      cpu->reg[operandA] = operandB;
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    default:
      printf("nope\n");
    }

    // 6. Move the PC to the next instruction.
    cpu->PC = cpu->PC + nextInstruction;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->reg, 0, 8);
  memset(cpu->ram, 0, 256);
}