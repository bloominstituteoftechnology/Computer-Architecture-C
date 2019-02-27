#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ================== RAM functions ===================

/**  Read from the CPU's ram at the specifc location 
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

/**  Writes at index specified for the CPU's ram
 */
void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char num)
{
  cpu->ram[MAR] = num;
}

// ================== Instrcution Handlers ===================

// LDI function: Set the value of a register to an integer
void handle_LDI(struct cpu *cpu)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char value = cpu_ram_read(cpu, (cpu->pc + 2));
  cpu->registers[regA] = value;
};

// PRN function: Print numeric value stored in the given register
void handle_PRN(struct cpu *cpu)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  printf("%d\n", cpu->registers[regA]);
}

// HLT function: Halt the CPU (and exit the emulator)
void handle_HLT()
{
  exit(0);
}

void handle_CALL(struct cpu *cpu)
{
  // Saves next PC at the correct spot
  cpu_ram_write(cpu, --cpu->registers[7], (cpu->pc + 2));

  char unsigned regA = cpu_ram_read(cpu, (cpu->pc + 1));
  cpu->pc = cpu->registers[regA];
}

void handle_RET(struct cpu *cpu)
{
  char unsigned newPc = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->pc = newPc;
}

// ================= Stack functions ==================

void handle_PUSH(struct cpu *cpu)
{
  char unsigned regA = cpu_ram_read(cpu, (cpu->pc + 1));
  cpu_ram_write(cpu, --cpu->registers[7], cpu->registers[regA]);
}

void handle_POP(struct cpu *cpu)
{
  char unsigned regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char value = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->registers[regA] = value;
}

// ================== CPU functions ===================

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv)
{
  FILE *ptr = fopen(argv, "r");
  char c[100];
  unsigned char ret;

  int address = 0;
  while (fgets(c, 100, ptr) != NULL)
  {
    char *endptr;
    ret = strtoul(c, &endptr, 2);
    if (endptr == c)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, ret);
  }

  fclose(ptr);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char regB = cpu_ram_read(cpu, (cpu->pc + 2));

  switch (op)
  {
  case ALU_MUL:
    // TODO
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];

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
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, (cpu->pc));

    switch (IR)
    {
    case LDI:
      handle_LDI(cpu);
      break;
    case PRN:
      handle_PRN(cpu);
      break;
    case HLT:
      handle_HLT();
      break;

    // Subroutine instructions
    case CALL:
      handle_CALL(cpu);
      continue;
    case RET:
      handle_RET(cpu);
      continue;

    // ALU Instructions
    case MUL:
      alu(cpu, ALU_MUL);
      break;
    case ADD:
      alu(cpu, ALU_ADD);
      break;

    //  Stack Instructions
    case PUSH:
      handle_PUSH(cpu);
      break;
    case POP:
      handle_POP(cpu);
      break;
    default:
      printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->pc);
      exit(1);
    }

    int opNum = (11000000 & IR) >> 6;
    cpu->pc += (opNum + 1);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xF4;
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
