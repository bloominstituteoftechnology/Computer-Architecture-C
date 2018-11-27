#include "cpu.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define DATA_LEN 6

unsigned char read_ram(struct cpu *cpu, unsigned char address)
  {
    return cpu->ram[address]; 
  }

  unsigned char write_ram(struct cpu *cpu, unsigned char address, unsigned char val)
  {
    return cpu->ram[address] = val;
  }

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename,"r");
  char line[1024];
  unsigned char address = 0; 

  if(fp == NULL){
    printf("Error opening file\n");
    exit(1);
  }

  // int mem_index = 0;
  while(fgets(line, sizeof line, fp) != NULL){
    // memory[mem_index] = strtoul(line, NULL, 10);
    // mem_index++;
    unsigned char b;
    b = strtoul(line, NULL, 2);
    write_ram(cpu, address++, b);
  }
  fclose(fp);
  // TODO: Replace this with something less hard-coded
}

/*
 * ALU
//  */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch(op){
    case ALU_MUL:
      cpu-> registers[regA] *= cpu-> registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char operandA = read_ram(cpu, cpu->PC + 1);
  unsigned char operandB = read_ram(cpu, cpu->PC + 2);

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char i =  read_ram(cpu, cpu->PC);
    switch(i)
    {
      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
      case HLT: 
        running = 0;
        cpu->PC++;
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
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
}

