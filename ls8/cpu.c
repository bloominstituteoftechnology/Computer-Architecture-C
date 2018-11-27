#include "cpu.h"
#include <stdio.h>

#define DATA_LEN 6

/**
 * Add RAM functions read and write
 */
 unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR) 
 {
   return cpu->ram[MAR];
 }

 void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
 {
   cpu->ram[MAR] = MDR;
 }

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(filename, "r");
  char line[8192];

  if (fp == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    cpu->ram[address++] = strtoul(line, NULL, 2);
  }
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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
    // 2. switch() over it to decide on a course of action.
    switch(IR) {

      case HLT:
        running = 0;
        break;

      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->PC += 3;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC += 2;
        break;

      default:
        printf("Default case triggered\n");
        break;

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
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
}
