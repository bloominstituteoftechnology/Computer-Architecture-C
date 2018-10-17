#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line(1024);
  unsigned char addr = 0x00;
    
  if (fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) !=NULL) {
    char *endptr;

    unsigned char b = strtoul(line, &endptr, 2);

    cpu_ram_write(cpu, addr++, b);
  }
  fclose(fp);
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
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff;

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB)

    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case PRN:
        print("%d\n", cpu->reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;
    }
    cpu->PC += add_to_pc;

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
  cpu->PC = 0;

  memset(cpu->ram, 0, sizeof cpu ->ram);
  memset(cpu->reg, 0, sizeof cpu ->reg);

  cpu->reg[7] = 0xF4
}
