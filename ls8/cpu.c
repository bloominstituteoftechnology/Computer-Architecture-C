
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6
#define SP 7

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  // mar = Memory Address registersister, holds the memory address
  // we're reading or writing

  // return value from ram specified by mar
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char mdr)
{
  // mdr = Memory Data registersister, holds the value
  // to write or the value just read

  // write mdr in ram
  cpu->ram[index] = mdr;
}

void cpu_push(struct cpu *cpu, unsigned char val)
{
  // Decrement SP
  cpu->registers[SP]--;
  // Copy the value in the given reg to the address pointed to by SP;
  cpu_ram_write(cpu, cpu->registers[SP], val);
}

unsigned char cpu_pop(struct cpu *cpu)
{
  // Read last value from stack
  unsigned char val = cpu_ram_read(cpu, cpu->registers[SP]);
  // Increment SP
  cpu->registers[SP]++;
  return val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "ls8: error opening file:  %s\n", filename);
    exit(2);
  }

  char line[8192]; // to hold individual lines in the file
  int address = 0;

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *endptr; // to keep track of non-numbers in the file
    // converts str to number
    unsigned char byte = strtoul(line, &endptr, 2);

    // prevents unnecessary lines being stored on ram
    if (endptr == line)
    {
      continue;
    }

    cpu->ram[address++] = byte;
  }

  fclose(fp);
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  // True until we get a HLT instruction
  int running = 1;
  unsigned char command, operand1, operand2;

  while (running)
  {
    command = cpu_ram_read(cpu, cpu->PC);

    operand1 = cpu_ram_read(cpu, cpu->PC + 1);
    operand2 = cpu_ram_read(cpu, cpu->PC + 2);

    switch (command)
    {
    case LDI:
      // sets value of operand1 to the registers[operand1]
      cpu->registers[operand1] = operand2;
      // Move the PC to the next instruction.
      cpu->PC += 3;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operand1]);
      // Move the PC to the next instruction.
      cpu->PC += 2;
      break;
    case MUL:
      cpu->registers[operand1] *= operand2;
      cpu->PC += 3;
      break;
    case ADD:
      cpu->registers[operand1] += operand2;
      cpu->PC += 3;
      break;

    case PUSH:
      cpu_push(cpu, cpu->registers[operand1]);
      cpu->PC += 2;
      break;

    case POP:
      // Copy the value from the address pointed to by SP to the given registers
      cpu->registers[operand1] = cpu_pop(cpu);
      cpu->PC += 2;
      break;

    case HLT:
      // Set running to false to stop program
      running = 0;
      // Move the PC to the next instruction.
      cpu->PC += 1;
      break;

    default:
      printf("KIT: I can't do that Michael\n");
      exit(1);
      break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // R0-R6 are cleared to 0
  for (int i = 0; i < 6; i++)
  {
    cpu->registers[i] = 0;
  }

  // R7 is set to 0xF4
  cpu->registers[7] = 0xF4;

  // PC is cleared to 0
  cpu->PC = 0;

  // RAM is cleared to 0
  memset(cpu->ram, 0, sizeof(cpu->ram));
}