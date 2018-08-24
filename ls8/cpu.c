#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char* arg, struct CPU *cpu)
{
  //original code :
  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000, // NOP
  //   0b00001000, //
  //   0b01000111, // PRN R0
  //   0b00000000, // NOP
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  //end original code

  FILE *fp;
  char line[1024];
  int line_no = 0;
  char *pointer;

  fp = fopen(arg, "r");

  if (fp == NULL) {
    fprintf(stderr, "Failed to open %s\n", arg);
    exit(1);
  }
  else {
    while(fgets(line, sizeof(line), fp)!= NULL) {
      
      unsigned char value = strtoul(line, &pointer, 2);
      if(pointer == line) {
        continue;
      }
      cpu_ram_write(cpu, line_no++, value);

    }
    fclose(fp);
  }

}

/**
 * ALU
 */
void alu(struct CPU *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg; 
  switch (op) {
    case ALU_MUL:
      reg[regA] *= reg[regB];
      break;
    case ALU_ADD:
      reg[regA] += reg[regB];
      break;
    case ALU_DIV:
      reg[regA] /= reg[regB];
      break;
    case ALU_SUB:
      reg[regA] -= reg[regB];
      break;
    case ALU_MOD:
      reg[regA] = reg[regA]%reg[regB];
      break;
  
    default:
      fprintf(stderr, "ALU_switch instruction error");
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct CPU *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char *reg = cpu->reg;
  unsigned char PC = cpu->PC;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
        unsigned char IR = cpu_ram_read(cpu, PC);
        unsigned char operandA = cpu_ram_read(cpu, (PC + 1));
        unsigned char operandB = cpu_ram_read(cpu, (PC + 2));
        int bitshift = ((IR >> 6)) + 1; 

    // 2. switch() over it to decide on a course of action.

    switch(IR)
    {
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        PC +=bitshift;
        break;
      case CALL:
        reg[SP] = reg[SP - 1];
        // cpu->ram[reg[SP]] = PC + bitshift;
        cpu_ram_write(cpu, reg[SP], PC + bitshift);
        PC = reg[operandA];
        break;
      case DEC:
        reg[operandA]--;
        printf("%d\n", PC);
        PC +=bitshift;
        break;
      
      case DIV:
        alu(cpu, ALU_DIV, operandA, operandB);
        PC +=bitshift;
        break;

      case HLT:
        running = 0;
        break;
      
      case INC:
        reg[operandA]++;
        PC += bitshift;
        break;
      
      case JMP:
        PC = reg[operandA];
        break;

      case LDI:
        reg[operandA] = operandB;
        PC+=bitshift;
        break;

      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        PC +=bitshift;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        PC+=bitshift;
        break;

      case POP:
        reg[operandA] = cpu_ram_read(cpu, reg[SP]++);
        PC +=bitshift;
        break;

      case PRN:
        printf("%d\n", reg[operandA]);
        PC+=bitshift;
        break;

      case PUSH:
        cpu_ram_write(cpu, --reg[SP], reg[operandA]);
        PC +=bitshift;
        break;
      case RET:
        PC = cpu_ram_read(cpu, reg[SP]++);
        break;
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        PC +=bitshift;
        break;

      default:
        printf("Unknown instruction at %02x: %02x\n", cpu->PC, IR);
        exit(2);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    #if DEBUG
    printf("\n**********REG in run************\n");
    for(unsigned long i = 0; i < 8; i++) {
      printf("cpu->reg[%lu] = %u\n", i, cpu->reg[i]);
    }
  #endif
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct CPU *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->MAR = 0;
  cpu->MDR = 0;
  cpu->FL = 0;
  cpu->reg[7] = 0xF4;
  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  #if DEBUG
    printf("\n**********REG in init************\n");
    for(unsigned long i = 0; i < 8; i++) {
      printf("cpu->reg[%lu] = %u\n", i, cpu->reg[i]);
    }
  #endif
}

// In `cpu.c`, add functions `cpu_ram_read()` and `cpu_ram_write()` that access the
// RAM inside the `struct cpu`.

// We'll make use of these helper function later.

// MAR = memory address ... holds the memory address of what we're reading or writing

// MDR = memory data register ... holds the VALUE to write or the VALUE just read

unsigned char cpu_ram_read(struct CPU *cpu, unsigned char index) 
{
  // TODO: access RAM and read it
  return cpu->ram[index];

}

void cpu_ram_write(struct CPU *cpu, unsigned char index, unsigned char value) 

 {
//   //TODO: access RAM to write to it. 
  cpu->ram[index] = value;

 }