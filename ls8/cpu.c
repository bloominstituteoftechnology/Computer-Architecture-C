#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

// #define DATA_LEN 6
#define SP 7

unsigned char cpu_pop(struct cpu *cpu)
{
  if (cpu->reg[7] == EMPTY_STACK)
  {
    fprintf(stderr, "Popping off is not allowed\n");
  }

  unsigned char value = cpu->ram[cpu->reg[SP]];

  cpu->reg[SP]++;

  return value;
}

void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->reg[SP]--;

  cpu->ram[cpu->reg[SP]] = value;
}

void cpu_load(char *filename, struct cpu *cpu)
{

  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  FILE *fp;
  char line[60];
  int address = PROGRAM_ENTRY;

  // Open the source file
  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  // Read all the lines and store them in RAM
  while (fgets(line, sizeof line, fp) != NULL)
  {

    // Convert string to a number
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);
    ;

    // Ignore lines from whicn no numbers were read
    if (endchar == line)
    {
      continue;
    }

    // Store in ram
    cpu->ram[address++] = byte;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;

  unsigned char valA = reg[regA];
  unsigned char valB = reg[regB];

  switch (op)
  {
  case ALU_MUL:
    valA *= valB;
    break;

  case ALU_ADD:
    valA += valB;
    break;

  case ALU_DIV:
    valA /= valB;
    break;

  case ALU_SUB:
    valA -= valB;
    break;

  case ALU_CMP:
    if (valA == valB)
    {
      // Set the EQ bit in the flags
      cpu->FL |= FLAG_EQ;
    }
    else
    {
      // Clear the EQ bit in the flags
      cpu->FL &= ~FLAG_EQ;
    }
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  unsigned char *ram = cpu->ram;

  int running = 1;

  while (running)
  {
    unsigned char curr_instruct = ram[cpu->PC];

    unsigned char value1 = ram[(cpu->PC + 1) & 0xff];
    unsigned char value2 = ram[(cpu->PC + 2) & 0xff];

    int inst_set_pc = (curr_instruct >> 4) & 1;

    switch (curr_instruct)
    {

    case LDI:
      reg[value1] = value2;
      break;

    case PRN:
      printf("%d\n", reg[value1]);
      break;

    case MUL:
      alu(cpu, ALU_MUL, value1, value2);
      break;

    case ADD:
      alu(cpu, ALU_ADD, value1, value2);
      break;

    case HLT:
      running = 0;
      break;

    case CALL:
      cpu_push(cpu, cpu->PC + 2);
      cpu->PC = reg[value1];
      break;

    case RET:
      cpu->PC = cpu_pop(cpu);
      break;

    case PUSH:
      cpu_push(cpu, reg[value1]);
      break;

    case POP:
      reg[value1] = cpu_pop(cpu);
      break;

    case CMP:
      alu(cpu, ALU_CMP, value1, value2);
      break;

    case JMP:
      cpu->PC = reg[value1];
      break;

    case JEQ:
      if (cpu->FL & FLAG_EQ)
      {
        cpu->PC = reg[value1];
      }
      else
      {
        inst_set_pc = 0;
      }
      break;

    case JNE:
      if (!(cpu->FL & FLAG_EQ))
      {
        cpu->PC = reg[value1];
      }
      else
      {
        inst_set_pc = 0;
      }
      break;

    default:
      fprintf(stderr, "error\n");
      printf("Current Instruction %u\n", curr_instruct);
      exit(1);
    }

    if (!inst_set_pc)
    {
      cpu->PC += ((curr_instruct >> 6) & 0x3) + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
  cpu->reg[7] = EMPTY_STACK;
}