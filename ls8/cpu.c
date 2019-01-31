#include "cpu.h"
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

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
 * ALU handles math and logic operations
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    case ALU_ADD:
      break;

    case ALU_INC:
      break;

    case ALU_DEC:
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

  // unsigned char IR, operandA, operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char operandA = NULL;
    unsigned char operandB = NULL;

    // unsigned char operandA, operandB;
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1)& 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2)& 0xff);
    } else if (num_operands == 1) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1)& 0xff);
    } else {}
    // 4. switch() over it to decide on a course of action.
    switch (instruction) {
      case HLT:
        running = 0;
        break;

      case PRN:
        // print whatever is in the specified register
        break;

      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      default:
        break;
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
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}


// So cpu_ram_read() and cpu_ram_write() access values from and modifies values in the ram array.
// Remember ram is going to be an array of numbers that is an attribute of the cpu struct.
// Even though it will be an array of numbers you’ll want to use the `unsigned char` type in your array
// because you’ll be working with relatively small numbers in this computer 
// (because of it’s limited memory holding capbilities). `cpu_ram_write` modifies the array so it 
// doesn’t need to return anything it just needs an index and a value to put into the array (edited) 
// `cpu_ram_read` on the other hand just reads from the ram array so it just needs an index as input
// so it knows which value to read and it returns the value in question as it’s output.