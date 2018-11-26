#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  /* char data[DATA_LEN] = {
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
  } */

  // TODO: Replace this with something less hard-coded

  FILE *fp;
  char data[1024];
  unsigned char address = 0;
  fp = fopen(argv[1], "r");
  
  if (fp == NULL) {
    perror("Can not open the file\n");
    exit(1);
  }
  
  while(fgets(data, sizeof(data), fp) != NULL) {
      unsigned char byte = strtoul(data, NULL, 2);
      if(data == NULL){
        continue;
      }
      cpu->ram[address++] = byte;
    }
  }

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;

  switch (op) {
    case ALU_MUL:
      // TODO
      reg[regA] *= reg[regB];
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

  unsigned char *reg = cpu->reg;
  unsigned char PC = cpu->PC;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, PC);
    unsigned char operandA = cpu_ram_read(cpu, (PC + 1));
    unsigned char operandB = cpu_ram_read(cpu, (PC + 2));
    int shift = ((IR >> 6)) + 1;

    // 2. switch() over it to decide on a course of action.
    switch (IR) {
      case HLT:
        running = 0;
        break;

      case LDI:
        reg[operandA] = operandB;
        PC += shift;
        break;

      case PRN:
        printf("%c", reg[operandA]);
        PC += shift;
        break;

      default:
        printf("Unrecognized instruction %02x: %02x\n", PC, IR);
        exit(2);
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

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  cpu->PC = 0x00;

}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {
  cpu->ram[index] = value;
}

