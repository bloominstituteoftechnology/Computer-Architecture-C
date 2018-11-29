#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {
  cpu->ram[index] = value;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  unsigned char address = 0;
  char new[1024];
  fp = fopen(argv[1], "r");
  if(fp == NULL) {
    printf("error \n");
    exit(1);
  }
  while (fgets(new, sizeof(new), fp) != NULL) {
    unsigned char value = strtoul(new, NULL, 2);
    if(new == NULL) {
      continue;
    }
    cpu -> ram[address++] = value;
  }
  fclose(fp);

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
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
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
    unsigned char ir = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    int shift = (ir >> 6) + 1;
    // 2. switch() over it to decide on a course of action.
    switch(ir) {
      case LDI:
      cpu->reg[operandA] = operandB;
      break;

      case PRN:
      printf("%d \n", cpu->reg[operandA]);
      break;

      case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

      case HLT: 
      running = 0;
      break;

     



    }
    cpu->PC += shift;
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
   memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
  cpu->reg[7] = 0xF4;
}

