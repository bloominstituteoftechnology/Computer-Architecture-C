#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"


unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu,char *filename)
{
  FILE *fp=fopen(filename,"r");
  if (fp==NULL) {
    printf("Could not open file");
  }
  int address=0;
  char string[256];
  while (fgets(string,sizeof(string),fp)!=NULL) {
    unsigned char data=strtol(string,NULL,2);
    cpu->ram[address++]=data;
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
    unsigned char IR, operandA, operandB;
    IR = cpu_ram_read(cpu, cpu->pc);
    operandA = cpu_ram_read(cpu, cpu->pc+1);
    operandB = cpu_ram_read(cpu, cpu->pc+2);
    int add_to_pc = (IR >> 6) + 1;

    switch (IR) {
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case HLT:
        running = 0;
        break;
    }
    cpu->pc += add_to_pc;
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
  cpu->pc = 0;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
  cpu->registers[7] = 0xF4;
}
