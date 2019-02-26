#include "cpu.h"
#include <string.h> 
#include <stdio.h>

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

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  (void)cpu;
  (void)regA;
  (void)regB;

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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    unsigned char ops = (IR & 0xC0) >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char op0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char op1 = cpu_ram_read(cpu, cpu->PC + 2);

    // 4. switch() over it to decide on a course of action.
    switch(IR) {

    // 5. Do whatever the instruction should do according to the spec.
      case LDI:
      cpu->registers[op0] = op1;
      break;

      case PRN:
      printf("%d\n", cpu->registers[op0]);
      break;
      
      case HLT:
      running = 0;
      break;
      
      default:
      printf("Unkown Instruction: 0x%02x\n", IR);
      running = 0;
      break;
    }

    // 6. Move the PC to the next instruction.
    cpu->PC += (ops + 1);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
