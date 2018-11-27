#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

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
  FILE *fp = fopen(filename, "r");
  int address = 0;
  char c[8192];
  while (fgets(c, sizeof c, fp) != NULL) {
   cpu->ram[address++] = strtoul(c, NULL, 2);
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
      break;

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char item)
{
  cpu->ram[index] = item;
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
    int PC = cpu->PC;
    unsigned char IR = cpu_ram_read(cpu, PC);
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case HLT:
        printf("HLT\n");
        running = 0;
        break;
      case LDI:
        printf("LDI\n");
        cpu->registers[(int)operandA] = operandB;
        break;
      case PRN:
        printf("PRN\n");
        printf("%d\n", cpu->registers[(int)operandA]);
        break;
      case MUL:
        printf("MUL\n");
        cpu->registers[(int)operandA] =  cpu->registers[(int)operandA] *  cpu->registers[(int)operandB];
        break;
      default:
        printf("%d", IR);
        printf("Default case reached.\n");
        break;
    }
    // 4. Move the PC to the next instruction.
    // printf("%d\n", (instruction >> 6));
    cpu->PC += 1 + (int)(IR >> 6);
    
  }
}