#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 256

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_name)
{
  int address = 0;
  char line[8192];

  FILE *fp = fopen(file_name,"r");

  while (fgets(line, sizeof line, fp) != NULL) {
    cpu->ram[address++] = strtoul(line, NULL, 2);
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
      printf("Multiply %d by %d\n", cpu->registers[regA], cpu->registers[regB]);
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
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
  unsigned char regA;
  unsigned char regB;
  int item;
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char ir = cpu->ram[cpu->PC];
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch (ir) {
      case LDI:
        regA = cpu_ram_read(cpu, cpu->PC+1);
        item = cpu_ram_read(cpu, cpu->PC+2);
        cpu->registers[regA] = item;
        break;
      case PRN:
        printf("%d\n", cpu->registers[cpu->ram[cpu->PC+1]]);
        cpu->PC++;
        break;
      case MUL:
        regA = cpu_ram_read(cpu, cpu->PC+1);
        regB = cpu_ram_read(cpu, cpu->PC+2);
        alu(cpu, ALU_MUL, regA, regB);
        break;
      case HLT:
      running = 0;
      break;
      default:
        break;
    }
    // 4. Move the PC to the next instruction.
    int movePC = (ir >> 6) + 1;
    cpu->PC += movePC;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = malloc(sizeof(struct cpu));
  cpu->PC = 0;
  memset(cpu->ram,0,256);

  for(int i = 0;i<7;i++){
    cpu->registers[i] = 0;
  }

  cpu->registers[7] = 0xF4;
}

int cpu_ram_read(struct cpu *cpu, int pc)
{
  return (unsigned char)cpu->ram[pc];
}

void cpu_ram_write(struct cpu *cpu, int item)
{
  cpu->ram[cpu->PC] = item;
  cpu->PC++;
}
