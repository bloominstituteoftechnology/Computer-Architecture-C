#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char i) {
  return cpu->ram[i];
}

void cpu_ram_write(struct cpu *cpu, unsigned char i, unsigned char val) {
  cpu->ram[i] = val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000, // 0
    0b00001000,
    0b01000111, // PRN R0
    0b00000000, // 0
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
    cpu->registers[regA] *= cpu->registers[regB];
    break;
    // TODO: implement more ALU ops
  case ALU_DIV:
    //add code
    cpu->registers[regA] /= cpu->registers[regB];
    break;
  case ALU_SUB:
    //add code
    cpu->registers[regA] -= cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    break;
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
    unsigned char curr = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    int add_1 = (curr>>6)+1;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xFF);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 1) & 0xFF);
    // 4. switch() over it to decide on a course of action.
    switch(curr) {
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      break;
    case HLT:
      running = 0;
      break;
    case MULT:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->PC++; //TODO: review this step
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);
}
