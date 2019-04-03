#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

// read what is in the ram
// `MAR`: Memory Address Register, holds the memory address we're reading or writing
int cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
};

// write to the ram
// * `MDR`: Memory Data Register, holds the value to write or the value just read
void cpu_ram_write(struct cpu *cpu, unsigned char val, unsigned char mdr)
{
  cpu->ram[mdr] = val;
};

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Missing arguments. Provide: ./ls8 filename\n");
    exit(1);
  }

  FILE *fp;
  char line[1024];
  char *file = argv[1];
  fp = fopen(file, "r");
  int address = 0;

  if (fp == NULL)
  {
    fprintf(stderr, "Error: unable to open file %s\n", file);
    exit(1);
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {

    char *ptr;
    unsigned char command = strtol(line, &ptr, 2);

    if (ptr == line)
    {
      continue;
    }

    cpu->ram[++address] = command;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB) // what are these?
{
  switch (op)
  {
  case ALU_MUL:
    //Multiply the values in two registers together and store the result in registerA.
    cpu->registers[regA] *= regB;
    break;
  case ALU_ADD:
    cpu->registers[regA] += regB;
    break;
  default:
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT command

  while (running)
  {
    // TODO
    // 1. Get the value of the current command (in address PC).
    unsigned char command = cpu->ram[cpu->PC];
    // 2. Figure out how many operands this next command requires
    unsigned int combined_operands = command >> 6;
    // 3. Get the appropriate value(s) of the operands following this command
    unsigned int operand1;
    unsigned int operand2;
    if (combined_operands == 2)
    {
      operand1 = cpu->ram[cpu->PC + 1];
      operand2 = cpu->ram[cpu->PC + 2];
    }
    else if (combined_operands == 1)
    {
      operand1 = cpu->ram[cpu->PC + 1];
    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the command should do according to the spec.
    switch (command)
    {
    case HLT:
      running = 0;
      break;

    case PRN:
      printf("%d\n", cpu->registers[operand1]);
      break;

    case LDI:
      cpu->registers[operand1] = operand2;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operand1, operand2);
      break;

    case POP:
      cpu->registers[operand1] = cpu_ram_read(cpu, cpu->registers[operand1]);
      break;

    case PUSH:
      // cpu->registers[7]--;
      cpu_ram_write(cpu, cpu->registers[operand2--]);
      break;

    default:
      break;
    }
    // 6. Move the PC to the next command.
    cpu->PC += combined_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // DONE✔:
  // Initialize the PC and other special registers
  // DONE✔: R0 - R6 are cleared to 0
  for (int i = 0; i < 6; i++)
  {
    // DONE✔: cpu->PC = '0';
    cpu->registers[i] = 0;
  }
  // DONE✔: R7 is set to 0xF4
  cpu->registers[7] = 0xF4;
  // DONE✔: PC is set to 0
  cpu->PC = 0;
  // DONE✔: RAM is set to 0
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
