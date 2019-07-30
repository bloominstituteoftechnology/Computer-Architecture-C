#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
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

  unsigned char reg_num1, reg_num2;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char ir = cpu->ram[cpu->pc];

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
        cpu->registers[0] = 8;
        break;

      // case NOP:
      //   break;
      // case POP:
      //   break;
      // case PRA:
      //   break;
      case PRN:
      // Print numeric value stored in the given register.
        printf("%d\n", cpu->registers[0]);
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