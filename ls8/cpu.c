#include "cpu.h"
#include "stdio.h"
#include "stdlib.h"

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
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
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
  unsigned char *registers = cpu->registers; // Short cut to registers
  unsigned char *ram = cpu->ram; // Short cut to ram

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+1);
    // 2. switch() over it to decide on a course of action.
    switch(IR) {
      // 3. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu_ram_write(cpu, operandA, operandB);
        cpu->PC += 3;
        printf("LDI instructions: %c", IR);
        break;
      default:
        printf("Unknown instructions: %c", IR);
        exit(0);
    }
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
  for (int i=0; cup->registers[i] != "\0"; i++) {
    cpu->registers[i] = 0;
  }
  for (int i=0; cup->ram[i] != "\0"; i++) {
    cpu->ram[i] = 0;
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, char index)
{
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, char index, char value)
{
  cpu->ram[index] = value;
}