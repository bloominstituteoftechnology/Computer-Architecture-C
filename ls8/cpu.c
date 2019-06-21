#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6


unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
} 


void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    fprintf(stderr, "error opening file");
    exit(2);
  }

  char line[8192];
  int address = 0;


  while (fgets(line, sizeof line, fp) != NULL) {

    char *endptr;
    unsigned char val = strtoul(line, &endptr, 2);

    if (endptr == NULL) {
      continue;
    }


    cpu->ram[address++] = val;
  } 

  fclose(fp);


}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{

  unsigned char firstValue = cpu->registers[regA];
  unsigned char secondValue = cpu->registers[regB];
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] = firstValue * secondValue;
      break;
    case ALU_ADD:
      cpu->registers[regA] = firstValue + secondValue;
      break;
    // TODO: implement more ALU ops
    case ALU_CMP:
      if (firstValue == secondValue) {
        cpu->FL = 0;
      }
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char ir = cpu_ram_read(cpu, cpu->pc);
    // 2. Figure out how many operands this next instruction requires
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    switch(ir) {

      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->pc += 3;
        break;
      case HLT:
        running = 0;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        cpu->pc += 2;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->pc += 3;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->pc += 3;
        break;
      //Jumps to the address stored in the given register
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        cpu->pc += 3;
        break;
      case JMP:
        cpu->pc = cpu->registers[operandA];
        cpu->pc += 2;
        break;
      ///IF equal flag is set to true jump to the address in the register
      case JEQ:
        if (cpu->FL == 1) {
          cpu->pc = cpu->registers[operandA];
          cpu->FL = 0;
        } else {
          cpu->pc += 2;
          cpu->FL = 0;
        }
        break;
      case JNE:
        if ((cpu->FL & 1) != 1) {
          cpu->pc = cpu->registers[operandA];
          cpu->FL = 0;
        } else {
          cpu->pc += 2;
          cpu->FL = 0;
        }
        break;
      default:
        running = 0;
        // break;
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
  cpu->pc = 0;
  cpu->FL = 0b00000000;
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xF4;
  // TODO: Initialize the PC and other special registers
  ///Initalize our cpu, start ram and registers and pc to 0 memset() might help
}


