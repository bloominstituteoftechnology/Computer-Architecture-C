#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  cpu->ram[MAR] = MDR;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  //Init the file
  FILE *fp = fopen(filename, "r");

  char line[1024];

  unsigned char mar = 0x00;

  //Check if the file exits:
  if (fp == NULL)
  {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(1);
  }

  //While there's lines in the file...
  while (fgets(line, sizeof line, fp) != NULL)
  {
    char *endptr;
    unsigned char machine_code = strtoul(line, &endptr, 2);

    if (endptr == line)
    {
      //we got no numbers
      continue;
    }

    //Write it into Memory
    cpu_ram_write(cpu, mar++, machine_code);
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    break;

  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1;

  unsigned char IR;

  while (running)
  {
    IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xFF);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xFF);

    int add_to_pc = (IR >> 6) + 1;
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB;
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case PUSH:
      cpu_ram_write(cpu, --cpu->reg[7], cpu->reg[operandA]);
      break;

    case POP:
      cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[7]++);
      break;

    case CALL:
      cpu_ram_write(cpu, --cpu->reg[7], ++cpu->PC);
      cpu->PC = cpu->reg[operandA];
      add_to_pc = 0;
      break;
    case RET:
      cpu->PC = cpu_ram_read(cpu, cpu->reg[7]++);
      break;
    case CMP:
      if (cpu->reg[operandA] == cpu->reg[operandB])
      {
        cpu->flag_reg[0] = 1;
      }
      else if (cpu->reg[operandA] > cpu->reg[operandB])
      {
        cpu->flag_reg[1] = 1;
      }
      else
      {
        cpu->flag_reg[2] = 1;
      }
      break;
    case JMP:
      cpu->PC = cpu->reg[operandA];
      add_to_pc = 0;
      break;
    case JEQ:
      if (cpu->flag_reg[0] == 1)
      {
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
      }
      break;
    case JNE:
      if (cpu->flag_reg[0] == 0)
      {
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
      }
      break;
    case HLT:
      running = 0;
      break;
    }
    cpu->PC += add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;

  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->flag_reg, 0, sizeof(cpu->flag_reg));

  cpu->reg[7] = 0b11110100;
}