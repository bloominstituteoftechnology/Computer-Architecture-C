#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];  // access RAM inside struct cpu
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;  // access RAM inside struct cpu
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{ 
    FILE * fp;
    fp = fopen(file, "r");
    char str[30];

    if (fp == NULL) {
      printf("Error: Could not open file\n");
      exit(1);
    }

    int address = 0;

    while( fgets(str, sizeof(str), fp) != NULL )
    {
        unsigned char binary;
        char *endptr;
        binary = strtoul(str, &endptr, 2);
        if (str != endptr) {
            cpu->ram[address++] = binary;
        }
    }
     //printf("cpu at address 1: %d\n\n", cpu_ram_read(cpu, 0));

    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
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
    // Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    int move_pc = (IR >> 6) + 1;

    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);

    switch (IR) {
            case HLT:
                running = 0;
                break;

            case LDI:
                cpu->registers[operandA] = operandB;
                cpu->PC += move_pc; // 3
                break;

            case PRN:
                printf("Saved value: %d\n", cpu->registers[operandA]);
                cpu->PC += move_pc; // 2
                break;

            case MUL:
                alu(cpu, ALU_MUL, operandA, operandB);
                cpu->PC += move_pc; // 3
                break;

            default:
              exit(1);
        }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Initialize the PC and other special registers
  cpu->PC = 0;
  // Zero registers and RAM
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
