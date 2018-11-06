#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar) {
  return cpu->RAM[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr) {
  cpu->RAM[mar] = mdr;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *prog_name)
{
  unsigned char address = 0x00;
  unsigned char b;
  char line[1024];
  FILE *fp;
  char full_prog_name[256];

  sprintf(full_prog_name, "./examples/%s", prog_name);
  fp = fopen(full_prog_name, "rb");
  while (fgets(line, sizeof line, fp) != NULL) {
    if ((line[0] == '\n') || (line[0] == '#')) continue;
    b = strtoul(line, NULL, 2);
    cpu->RAM[address++] = b;
  }
  fclose(fp);  
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->REG[regA] = (cpu->REG[regA] * cpu->REG[regB]) & 0xff;
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
  unsigned char PC, IR, SP, operandA, operandB;
  SP = cpu->REG[7];

  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    PC = cpu->PC;
    IR = cpu_ram_read(cpu, PC);
    operandA = cpu_ram_read(cpu, PC + 1) & 0xff;
    operandB = cpu_ram_read(cpu, PC + 2) & 0xff;

    printf("TRACE: %02X: %02X, %02X, %02X, %02X\n", PC, IR, operandA, operandB, SP);

    // 2. switch() over it to decide on a course of action.
    switch(IR) {
    
    // 3. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu->REG[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->REG[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case PUSH:
        SP -= 1;
        cpu_ram_write(cpu, SP, cpu->REG[operandA]);
        cpu->REG[7] = SP;
        break;
      case POP:
        cpu->REG[operandA] = cpu_ram_read(cpu, SP);
        SP += 1;
        cpu->REG[7] = SP;
        break; 
      case HLT:
        running = 0;
        break;
    }
    // 4. Move the PC to the next instruction.
    cpu->PC = PC + (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0x00;
  cpu->FL = 0x00;
  
  memset(cpu->RAM, 0, sizeof cpu->RAM);
  memset(cpu->REG, 0, sizeof cpu->REG);
  cpu->REG[7] = 0xf4;
}

void cpu_free(struct cpu *cpu) {
  free(cpu);
}
