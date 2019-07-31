#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void trace(struct cpu *cpu);

void cpu_load(struct cpu *cpu, char *file_name)
{

  FILE *fp = fopen(file_name, "r");

  if (fp == NULL) {
    fprintf(stderr, "Error opening file: %s\n", file_name);
    return;
  }

  int address = 0;
  char line[128];

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr;

    unsigned char value = strtoul(line, &endptr, 2);

    if (endptr == line) {
      continue;
    }

    cpu->ram[address++] = value;
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
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
    // ADD, AND, CMP, DEC, DIV, INC, MOD, MUL, NOT, OR, SHL, SHR, SUB, XOR
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char operandA, operandB;

  while (running) {
    //trace(cpu);
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char ir = cpu->ram[cpu->pc];

    operandA = cpu->ram[cpu->pc+1];
    operandB = cpu->ram[cpu->pc+2];

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction

    // 4. switch() over it to decide on a course of action.

    switch (ir) {
      // case CALL:
      //   break;
      case HLT:
        running = 0;
        cpu->pc++;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->pc += 3;
        break;

      // case INT:
      //   break;
      // case IRET:
      //   break;
      // case JEQ:
      //   break;
      // case JGE:
      //   break;
      // case JGT:
      //   break;
      // case JLE:
      //   break;
      // case JLT:
      //   break;
      // case JMP:
      //   break;
      // case JNE:
      //   break;
      // case LD:
      //   break;
      case LDI:
        // Set the value of a register to an integer.
        cpu->registers[operandA] = operandB;
        cpu->pc += 3;
        break;

      // case NOP:
      //   break;
      // case POP:
      //   break;
      // case PRA:
      //   break;
      case PRN:
      // Print numeric value stored in the given register.
        printf("%d\n", cpu->registers[operandA]);
        cpu->pc += 2;
      // Print to the console the decimal integer value that is stored in the given register.
        break;

      // case PUSH:
      //   break;
      // case RET:
      //   break;
      // case ST:
      //   break;
    }


    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}

char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char input)
{

  int index = 0;
  int running = 1;

  while (running) {

    if (cpu->ram[index] == 0b00000001) {

      if (index < 256) {
        cpu->ram[index + 1] = 0b00000001;
        cpu->ram[index] = input;
      } else {
        cpu->ram[index - 1] = input;
      }
      running = 0;
      break;
    }
  }
}

void trace(struct cpu *cpu)
{
    printf("%02X | ", cpu->pc);

    printf("%02X %02X %02X |",
        cpu_ram_read(cpu, cpu->pc),
        cpu_ram_read(cpu, cpu->pc + 1),
        cpu_ram_read(cpu, cpu->pc + 2));

    for (int i = 0; i < 8; i++) {
        printf(" %02X", cpu->registers[i]);
    }

    printf("\n");
}