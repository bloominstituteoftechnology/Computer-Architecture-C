#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char item)
{
  cpu->ram[index] = item;
}

void cpu_load(char* filename, struct cpu *cpu)
{
  FILE *fp = fopen(filename, "r" );
  char line[1024];
  if(fp == NULL) {
    printf("Error: Could not open file.\n");
    exit(1);
  }
  int address = 0;
  
  while(fgets(line, sizeof line, fp) != NULL) {
    cpu->ram[address] = strtoul(line, NULL, 2);
    address++;
  }
  fclose(fp);

  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  

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
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char IR;
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC +=3;
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
  // cpu->registers[7] = 0xF4;

  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
