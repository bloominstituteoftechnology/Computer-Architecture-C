#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

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

  // TODO: Replace this with something less hard-codedmake
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  // mdr is value to set to ram
  // mar is the index of where the data is or will be
  // return mar which is the index of data in the ram
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  // write mdr to ram
  cpu->ram[mar] = mdr;
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
    
    // 2. Figure out how many operands this next instruction requires
    
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 1);

    // print out hex letters, 2 means it'll be two characters long
    printf("TRACE: %02X   %02X   %02X   %02X\n", cpu->PC, IR, operandA, operandB);
    
    // 4. switch() over it to decide on a course of action.
  switch(IR) {
    case LDI:
      cpu->reg[operandA] = operandB;
      cpu->PC += 3;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      cpu->PC += 2;
      break;
    case HLT:
      running = 0;
      break;
    default:
      printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->PC);
      exit(1);
  }

    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    
    break;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  for(int i = 0; i <= 6; i++) {
    cpu->reg[i] = 0;
  }
  // * `R0`-`R6` are cleared to `0`.
  cpu->reg[7] = 0xF4;

  cpu->PC = 0;
  cpu->FL = 0;
  // resets all ram in the array to 0
  memset(cpu->ram, 0, sizeof(unsigned char)*256);
  // PC cpu
}
