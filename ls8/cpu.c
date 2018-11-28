#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  //read from memory address
  return cpu->ram[mar];
}

unsigned char cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  //write value to memory
  cpu->ram[mar] = mdr;
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

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *fp = fopen(argv[1], "r");
  char str[256];
  int address = 0;
  char *ptr;

  // take input, and returns a string into a number, to be assigned as an address
  // assigning the file an address
  while (fgets(str, sizeof(str), fp) != NULL)
  {
    // taking information from string, converting to binary (base 2)
    unsigned char data = strtoul(str, &ptr, 2);
    if (ptr == str)
    {
      continue;
    }
    cpu->ram[address++] = data;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->registers;

  switch (op)
  {
  case ALU_MUL:
    // TODO
    reg[regA] *= reg[regB];
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
    unsigned char ir = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 2. switch() over it to decide on a course of action.
    switch (ir)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      cpu->PC += 3;
      break;
    case PRN:
      printf("%u\n", cpu->registers[operandA]);
      cpu->PC += 2;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      cpu->PC += 3;
      break;
    case HLT:
      running = 0;
      break;
    default:
      exit(1);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
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
  //memset(starting address of memory to be filled, value to be filled, Number of bytes to be filled starting at address[0])
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
