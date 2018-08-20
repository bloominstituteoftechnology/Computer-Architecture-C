#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  const int DATA_LEN = 6;
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
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

unsigned char read_cpu_ram(struct cpu *cpu, int location)
{
  return cpu->ram[location];
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {

    // unsigned char IR = cpu->ram[cpu->PC];
    // unsigned char operandA = cpu->ram[cpu->PC + 1];
    // unsigned char operandB = cpu->ram[cpu->PC + 2];
    unsigned char IR = read_cpu_ram(cpu, cpu->PC);
    unsigned char operandA = read_cpu_ram(cpu, cpu->PC + 1);
    unsigned char operandB = read_cpu_ram(cpu, cpu->PC + 2);

    switch (IR){
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;
      case HLT:
        running = 0;
        break;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    printf("what PC is = %d\n", cpu->PC);
    printf("what IR is = %d\n", IR);
    cpu->PC += 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
