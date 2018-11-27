#include "cpu.h"
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>




/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */


void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[9999];

  if(fp == NULL) {
    printf("Error opening file\n");
    exit(1); 
  }

  int address = 0;

  while(fgets(line, sizeof(line), fp) != NULL) {
    cpu->ram[address] = strtoul(line, NULL, 2); 
    address++; 
  }
  
  fclose(fp);
}

  // TODO: Replace this with something less hard-coded


unsigned char cpu_ram_read(struct cpu *cpu, unsigned char PC) 
{
  return cpu->ram[PC]; 
}

void cpu_ram_write(struct cpu *cpu, unsigned char PC, unsigned char input) 
{
  cpu->ram[PC] = input; 
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->R[0] = regA * regB;
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
    int c = cpu->ram[cpu->PC]; 
    // 2. switch() over it to decide on a course of action.
    unsigned char movePC = (cpu->ram[cpu->PC] >> 6);

    switch (c) {
      case LDI: 
        cpu->R[cpu->ram[cpu->PC + movePC - 1]] = cpu->ram[cpu->PC + movePC];
        cpu->PC += movePC;
        break;

      case PRN:
        printf("R[0] Hex: %x\n", cpu->R[cpu->ram[cpu->PC + movePC]]);
        printf("R[0] Decimal: %d\n", cpu->R[cpu->ram[cpu->PC + movePC]]);
        cpu->PC += movePC; 
        break;

      case MUL:
        alu(cpu, 0, cpu->R[cpu->ram[cpu->PC + movePC - 1]], cpu->R[cpu->ram[cpu->PC + movePC]]);
        printf("R[0] after ALU runs: %d\n", cpu->R[0]);
        cpu->PC += movePC;
        break;

      case HLT:
        running = 0;
        break; 
    }
    cpu->PC += 1; 
    
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
  memset(cpu->R, 0, 7 * sizeof(cpu->R[0]));
  cpu->R[7] = 0xF4; 
  memset(cpu->ram, 0, 256 * sizeof(cpu->ram[0]));
}


