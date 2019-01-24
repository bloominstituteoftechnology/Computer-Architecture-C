#include "cpu.h"
#include <string.h>
#include <stdio.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  FILE *fp;
  char line[1024];

  if((fp = fopen("print8.ls8", "r")) == NULL) {

  }
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
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  // Just so we don't have to type cpu-> every time
  unsigned char *reg = cpu->reg;
  unsigned char *ram = cpu->ram;

  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char operandA = NULL;
    unsigned char operandB = NULL;
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned int num_operands = IR >> 6;

    // &'ing by 0xff is essentially modding by 0xff, or 255
    // This just ensures that we wrap around if we ever reach
    // the end of the 255 bytes of allocated RAM and don't go
    // past it
    if (num_operands == 2) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    } else if (num_operands == 1) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    } else {}

    // True if this instruction might set the PC
    // This line is shifting the instruction by 4 bits to access
    // the flag that indicates whether the PC might be set, and
    // then &'ing it to see if the bit is set to 0 or 1
    int instruction_set_pc = (IR >> 4) & 1;

    switch (IR) {

      case LDI:
        reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;

      default:
        fprintf(stderr, "PC %02x: unknown instruction %02x\n", cpu->PC, IR);
        exit(3);
    }

    if (!instruction_set_pc) {
      // Increment PC by the number of arguments that were passed
      // to the instruction we just executed
      // We do this by shifting 6 bits and modding by 4 to access
      // the 1st and 2nd bits of the IR, which indicate how many
      // operands the previous instruction expected
      // Plus 1 because that is the size of the opcode itself
      cpu->PC += num_operands + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->reg));
  memset(cpu->reg, 0, sizeof(cpu->ram));
}

/**
 * Helper function to write a value to the specified index in RAM
 */
void cpu_ram_write(struct cpu *cpu, unsigned char val, unsigned char index)
{
  cpu->ram[index] = val;
}

/**
 * Helper function to read a value from the specified index in RAM
 * Returns the read value
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}
