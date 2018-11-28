#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[8888];

  if (fp == NULL) {
    perror("Error opening file. \n");
    exit(1);
  }

  int address = 0;

  while (fgets(line, sizeof line, fp) != NULL) {
    cpu->ram[address] = strtoul(line, NULL, 2);
    address++;
  }

  /*
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
  }*/

  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, char index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, char index, unsigned char val) {
  cpu->ram[index] = val;
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
  unsigned char IR;
  // unsigned char registers_index;
  // int num;
  unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
  unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);


  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. switch() over it to decide on a course of action.
    switch(IR) {
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
      case HLT:
        running = 0;
        break;
      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;
      case PRN:
        printf(">> register value: %d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += 3;
        break;
      case ADD:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += 3;
        break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // R7 is set to 0xF4
  // PC and FL registers are cleared to 0
  cpu->PC = 0;
  memset(cpu->registers,0,7);
  cpu->registers[7] = 0xF4;
  // TODO: Zero registers and RAM
  memset(cpu->ram,0,256);
}
