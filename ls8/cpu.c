#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  int index = 0;

  if ((fp = fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL)
  {
    unsigned char code = strtol(line, NULL, 2);

    // check for weirdness
    if (line[0] == '\n' || line[0] == '#')
    {
      continue;
    }

    cpu->ram[index++] = code;
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

    // TODO: implement more ALU ops
  }
}

// index would traditionally be called mar - memory address register
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

// value would traditionally be called mdr - memory data register
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value)
{
  cpu->ram[index] = value;
}

/**
* Trace function
*/
void trace(struct cpu *cpu)
{
  unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1)) & 0xff;
  unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2)) & 0xff;
  unsigned char IR = IR = cpu_ram_read(cpu, cpu->PC);
  char instruction[50];

  printf("\tTRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

  for (int i = 0; i < 8; i++)
  {
    printf(" %02X", cpu->reg[i]);
  }
  switch (IR)
  {
  case 0x82:
    sprintf(instruction, "%s R%d %02X", "LDI", operandA, operandB);
    break;

  case 0x47:
    sprintf(instruction, "%s R%d", "PRN", operandA);
    break;

  case 0xA2:
    sprintf(instruction, "%s R%d R%d", "MUL", operandA, operandB);
    break;

  case 0x01:
    sprintf(instruction, "%s", "HLT");
    break;

  default:
    sprintf(instruction, "%s", "unknown");
    break;
  }
  printf(" | %s", instruction);
  printf("\n");
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu, _Bool show_trace)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;

  while (running)
  {
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, (cpu->PC + 1)) & 0xff; // incase PC > 0xFF
    operandB = cpu_ram_read(cpu, (cpu->PC + 2)) & 0xff; // incase PC > 0xFF

    if (show_trace)
    {
      trace(cpu);
    }
    // printf("\tPC:%02X\tIR:%02X\tA:%02X\tB:%02X\n", cpu->PC, IR, operandA, operandB);

    // get the number of operands and add 1 (for the opcode)
    // AABCDDDD AA is # of operands
    int add_to_pc = (IR >> 6) + 1;

    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    // 3. Do whatever the instruction should do according to the spec.
    case HLT:
      running = 0;
      break;

    case LDI:
      cpu->reg[operandA] = operandB;
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    default:
      printf("\nBad code\n\n");
      exit(1);
    }
    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
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
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
}
