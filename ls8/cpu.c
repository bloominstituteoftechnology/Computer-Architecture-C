#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  (void)cpu;
  (void)regA;
  (void)regB;
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  return cpu->ram[MAR] = MDR;
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
    unsigned char ir = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
    // 4. switch() over it to decide on a course of action.
    switch (ir)
    {
    case HLT:
      running = 0;
      break;
    case LDI:
      cpu->registers[operandA] = operandB;
      cpu->pc += 3;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      cpu->pc += 2;
      break;
    default:
      printf("Unknown instruction 0x%02X at 0x%02X\n", ir, cpu->pc);
      exit(1);
    }
  }

  // 5. Do whatever the instruction should do according to the spec.
  // 6. Move the PC to the next instruction.
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // Init Registers
  for (int i = 0; i < 7; i++)
  {
    cpu->registers[i] = 0;
  }

  cpu->registers[7] = 0xF4;
  // Init PC
  cpu->pc = 0;
  // Init Ram
  memset(cpu->ram, 0, sizeof(cpu->ram));

  // memset(cpu->registers, 0, 256 * sizeof(char));
  // return 0;
}
