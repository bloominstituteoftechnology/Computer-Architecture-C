#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filen, struct cpu *cpu)
{
  // const unsigned int DATA_LEN = 6;
  // char data[6] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < 6; i++) {
  //   cpu->ram[address++] = data[i];
  // }
  FILE *fp;
  char line[1024];
  int address;

  if ((fp = fopen(filen, "r")) == NULL) {
    fprintf(stderr, "Error locating file %s\n", filen);
    exit(1);
  }

    while (fgets(line, sizeof line, fp) != NULL) {
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);;
  // TODO: Replace this with something less hard-coded

      if (endchar == line) {
            continue;
          }

      cpu->ram[address++] = byte;
    }
}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->registers;

  switch (op) {
    case ALU_MUL:
    reg[regA] *= reg[regB];
      break;
    case ALU_ADD:
    reg[regA] += reg[regB];
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

unsigned char cpu_ram_write(struct cpu *cpu, unsigned char write, unsigned char address) {
  cpu->ram[address] = write;
  return write;
}

void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
        unsigned char IR = cpu->ram[cpu->PC]; //cpu_ram_read(cpu, cpu->PC)

        unsigned char operandA = cpu->ram[(cpu->PC + 1) & 0xff]; //cpu_ram_read(cpu, cpu->PC + 1)
        unsigned char operandB = cpu->ram[(cpu->PC + 2) & 0xff]; //cpu_ram_read(cpu, cpu->PC + 2)
    // 2. switch() over it to decide on a course of action.
        int instruction_set_pc = (IR >> 4) & 1;

        // void handle_LDI(operandA, operandB) 
        // {
        //   cpu->registers[operandA] = operandB;
        // }

        // void handle_HLT(operandA, operandB)
        // {
        //   exit(1);
        // }
        // void handle_PRN(operandA, operandB) 

        // void handle_MUL(operandA, operandB) 


        switch(IR) {
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

          default:
            printf("unknown instruction at %02x: %02x\n", cpu->PC, IR);
            break;
        }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.+
    if (!instruction_set_pc) {
      cpu->PC += ((IR >> 6) & 0x3) + 1;
    }
 }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
