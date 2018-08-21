#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

// Beej
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}



/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  const int DATA_LEN = 6;
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8   // instruction
    0b00000000, // operand 1
    0b00001000, // operand 2
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
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    //=========== part 1 solution ==============


    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC +1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC +2);
    printf("TRACE: %02x: %02x\n", cpu->PC, IR);

    // ============ part 1 end solution =============

    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC += 2;
        break;
      case HLT:
        running = 0;
        break;
      case MUL:
        
        break;

      default:
        printf("unknown in Instructions");
    }
  }
}


char array[200];

void read_file(char *filename) {
  FILE * fPointer;
  fPointer = fopen(filename, "r");

  while(!feof(fPointer)) {
    fgets(array, 200, fPointer);
    puts(array);
  }
  fclose(fPointer);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC=0;
  read_file("examples/mult.ls8");
  // TODO: Zero registers and RAM
}

