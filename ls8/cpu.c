#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *argv[])
{

  if (argc < 2)
  {
    printf("File does not exist.\n");
    exit(1);
  }

  char *file = argv[1];


  FILE *fp = fopen(file, "r");


  if (fp == NULL)
  {
    printf("File does not exist");
    exit(1);
  }
  else
  {
    char file_line[1024];
    int address = 0;

    while (fgets(file_line, sizeof(file_line), fp) != NULL)
    {

      char *endptr;
      unsigned char val = strtol(file_line, &endptr, 2);


      if (file_line == NULL)
      {
        continue;
      }

      cpu->ram[address] = val;
      address++;
    }
  }

  fclose(fp);
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index )
{
  return cpu->ram[index];
}

unsigned char cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  return cpu->ram[index] = value;
}



/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;

    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;
  }
}

void push_stack(struct cpu *cpu, int val)
{
  cpu->registers[7]--;
  cpu_ram_write(cpu, cpu->registers[7], val);
}

unsigned char pop_stack(struct cpu *cpu)
{
  unsigned char val = cpu->ram[cpu->registers[7]];
  cpu->registers[7]++;
  return val;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char operandA;
  unsigned char operandB;


  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = IR >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    }
    else if (num_operands == 1)
    {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    }
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
      case HLT:
        running = 0;
        break;

      case LDI:
        cpu->registers[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case PUSH:
        push_stack(cpu, cpu->registers[operandA]);
        break;

      case POP:
        cpu->registers[operandA] = pop_stack(cpu);
        break;

      case CALL:
        push_stack(cpu, cpu->PC + 1);
        cpu->PC = cpu->registers[operandA] - 1;
        break;

      case RET:
        cpu->PC = pop_stack(cpu);
        break;
      
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      

      default:
        printf("Error command doesn't exist\n");
        exit(1);
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));

  int total = sizeof(cpu->ram) / sizeof(unsigned char);
  cpu->registers[7] = total;
}

