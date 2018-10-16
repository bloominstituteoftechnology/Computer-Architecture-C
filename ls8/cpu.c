#include "cpu.h"

#define DATA_LEN 6


unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

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
    cpu_ram_write(cpu, address++, data[i]);
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
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operand_a = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    unsigned char operand_b = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    int add_to_pc = (IR >> 6) + 1;
    printf("PC is at %d\n\n", cpu->PC);

    switch (IR)
    {
    case LDI:
      printf("LDI: CPU stored a value\n\n");
      cpu_ram_write(cpu, operand_a, operand_b);
      break;
    case PRN:
      printf("PRN: Print value %d\n\n", cpu->ram[0]);
      break;
    case HLT:
      printf("HLT: Program has halted\n\n");
      running = 0;
      exit(0);
    default:
      printf("instruction does not exist\n\n");
      exit(1);
    }
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
  memset(cpu->ram,0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);

  cpu->registers[7] = 0b11110100;
}
