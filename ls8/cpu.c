#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6


unsigned char  cpu_ram_read(struct cpu *cpu, unsigned char i)
{
  return cpu->ram[i];
}

void cpu_ram_write(struct cpu *cpu, unsigned char i, unsigned char data)
{
  cpu->ram[i] = data;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr= 0x00;

  if (fp == NULL) {
    fprintf(stderr, "eror opening file %s\n", filename);
    exit(2);
  }
  while(fgets(line, sizeof line, fp)!=NULL) {
    unsigned char b = strtoul(line, NULL, 2);

    cpu_ram_write(cpu, addr++, b);
  }
  fclose(fp);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  (void)cpu;
  (void)regA;
  (void)regB;

  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
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

  IR = cpu_ram_read(cpu, cpu->pc);
  operandA = cpu_ram_read(cpu, cpu->pc+1);
  operandB = cpu_ram_read(cpu, cpu->pc+2);

  int add_to_pc = (IR >> 6) +1;

  while (running) {
    

    switch(IR){
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case HLT:
        running = 0;
        break;
    }
    cpu->pc += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->pc = 0;
    memset(cpu->ram, 0, sizeof cpu->ram);
    memset(cpu->registers, 0, sizeof cpu->registers);
    cpu->registers[7] = 0xF4;
}

