#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include "cpu.h"


#define DATA_LEN 6


unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
 cpu->ram[MAR] = MDR;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  char line[1024];
  int address = 0;

  FILE *fp = fopen(filename, "r");
  //printf("%s\n", filename);
  if (fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  while(fgets(line, sizeof line, fp) != NULL) {
    //puts(line);
    char *endchar;
    unsigned char data = strtoul(line, &endchar, 2);

    if (line == endchar) {
      continue;
    }
    //printf("%u\n", data);
    cpu_ram_write(cpu, address++, data);
  }
  fclose(fp);
}


//   char data[DATA_LEN] = {
//     // From print8.ls8
//     0b10000010, // LDI R0,8
//     0b00000000,
//     0b00001000,
//     0b01000111, // PRN R0
//     0b00000000,
//     0b00000001  // HLT
//   };

//   //int address = 0;

//   for (int i = 0; i < DATA_LEN; i++) {
//     cpu->ram[address++] = data[i];
//   }

//   // TODO: Replace this with something less hard-coded
// }

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:

      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;

  while (running) {
    
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->pc);

    operandA = cpu_ram_read(cpu, cpu->pc + 1);
    operandB = cpu_ram_read(cpu, cpu->pc + 2);

    //int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02x: %02x\n", cpu->pc, IR);
    //printf("TRACE: %02X: %02X %02X %02X\n", cpu->pc, IR, operandA, operandB);
    // 2. switch() over it to decide on a course of action.
    switch(IR) {

      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->pc += 3;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->pc += 2;
        break;
      case HLT:
        running = 0;
        cpu->pc += 1;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->pc += 3;
        break;
      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;

  // TODO: Zero registers and RAM
  //memset(cpu->ram, 0, sizeof cpu->ram);
  //memset(cpu->registers, 0, sizeof cpu->registers);

  //cpu->registers[7] = 0xF4;
}
