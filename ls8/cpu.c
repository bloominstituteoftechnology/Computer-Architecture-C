#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cpu.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
// void cpu_load(struct cpu *cpu)
// {
//   const int DATA_LEN = 6;
//   char data[6] = {
//     // From print8.ls8
//     0b10000010, // LDI R0,8
//     0b00000000,
//     0b00001000,
//     0b01000111, // PRN R0
//     0b00000000,
//     0b00000001  // HLT
//   };

//   unsigned char address = 0;

//   for (int i = 0; i < DATA_LEN; i++) {
//     cpu->ram[address++] = data[i];
//   }

//   // TODO: Replace this with something less hard-coded
// }

void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp;
  char line[1024];
  unsigned char address = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {

    char *end;
    unsigned char byte = strtoul(line, &end, 2);

    if (end == line) {
      continue;
    }

    cpu->ram[address++] = byte;
  }
}


// read RAM
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// write RAM
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  return cpu->ram[address] = value;
}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] *= cpu->registers[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR; // Instruction Register, contains a copy of the currently executing instruction
  unsigned char IR_size;
  unsigned char MAR; // Memory Address Register, holds the memory address we're reading or writing
  unsigned char MDR; // Memory Data Register, holds the value to write or the value just read

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu,cpu->PC);
    // printf("Instrucction machine code %x PC %d\n",IR, cpu->PC);
    IR_size = (IR >> 6) + 1; // Get the instructions size
    switch(IR_size) {
      case 3:
        MDR = cpu_ram_read(cpu, cpu->PC + 2);
      case 2:
        MAR = cpu_ram_read(cpu, cpu->PC + 1);
        break;
    }
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case HLT:
        printf("HLT : %x\n",IR);
        running=0;
        break;

      case RET:
        printf("RET : %x\n",IR);
        cpu->PC = cpu_ram_read(cpu,cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case LDI:
        printf("LDI : %x R%d %d\n",IR, MAR, MDR);
        cpu->registers[MAR] = MDR;
        break;

      case PRN:
        printf("PRN : %x R%d\n",IR, MAR);
        printf("%d\n", cpu->registers[MAR]);
        break;

      case MUL:
        printf("MUL : %x R%d %d\n",IR, MAR, MDR);
        alu(cpu,ALU_MUL,MAR, MDR);
        break;

      case ADD:
        printf("ADD : %x R%d %d\n",IR, MAR, MDR);
        alu(cpu,ALU_ADD,MAR, MDR);
        break;

      case PUSH:
        printf("PUSH : %x R%d\n",IR, MAR);
        cpu->registers[7]--;
        cpu_ram_write(cpu,cpu->registers[7],cpu->registers[MAR]);
        break;

      case POP:
        printf("POP : %x R%d\n",IR, MAR);
        cpu->registers[MAR] = cpu_ram_read(cpu,cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case CALL:
        printf("CALL : %x R%d\n",IR, MAR);
        cpu->registers[7]--;
        cpu_ram_write(cpu,cpu->registers[7],cpu->PC + IR_size -1);
        cpu->PC = cpu->registers[MAR] - IR_size;
        break;

      default:
        printf("instruction doesn't exist: %x\n",IR);
        // exit(2);
        break;
    }
    // 4. Move the PC to the next instruction.
    cpu->PC += IR_size;
    // printf("cpu->PC %d\n",cpu->PC);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->registers[7] = 0xf4; // The SP points at the value at the top of the stack (most recently pushed), or at address F4 if the stack is empty.

  // TODO: Zero registers and RAM
  // Zero registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);
}