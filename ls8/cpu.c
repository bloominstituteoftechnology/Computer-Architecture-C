#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

void cpu_load(struct cpu *cpu, char *filename)
{
  char data[1024];
  int address = 0;
  FILE *fp = fopen(filename, "r");

  while(fgets(data, sizeof data, fp) != NULL)
  {
    if (data[0] == '\n' || data[0] == '#')
    {
      continue;
    }
    cpu_ram_write(cpu, address++, strtoul(data, NULL, 2));
  }
  fclose(fp);
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;
  }
}

void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running) {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1)) & 0xff;
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2)) & 0xff;
    int add_to_pc = (IR >> 6) + 1;

    switch(IR) {
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
    cpu->PC += add_to_pc;
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);
}
