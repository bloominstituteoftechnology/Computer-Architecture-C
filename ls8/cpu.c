#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  return cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu)
{
  FILE *fileptr;
  // char filename = "./examples/print8.ls8";
  char newdata[1024];
  fileptr = fopen("print8.ls8", "r");
    while (fgets(newdata, sizeof newdata, fileptr) != NULL) {
    printf("%s\n", newdata);
    }


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
  unsigned char IR, operandA, operandB;
  
  while (running) {
    IR = cpu_ram_read(cpu, cpu->pc);
    operandA = cpu_ram_read(cpu, cpu->pc+1);
    operandB = cpu_ram_read(cpu, cpu->pc+2);
    printf("TRACE: %02X: %02X %02X %02X\n", cpu->pc, IR, operandA, operandB);
    switch(IR) {
      case LDI:
        cpu_ram_write(cpu, operandA, operandB);
        printf("writing to ram");
        break;

      case PRN:
        printf("%d\n", cpu_ram_read(cpu, operandA));
        break;

      case HLT:
        printf("stopping");
        running = 0;
        break;

      default:
      break;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
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
  cpu->pc = 0;

  memset(cpu->ram, 0, sizeof cpu-> ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
}
