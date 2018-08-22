#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


  
  // TODO: cpu.c cpu_load:  Replace this with something less hard-coded
  // TODO: cpu.c line 41: implement ALU ops
  // TODO: cpu.c while running:
          // 1. Get the value of the current instruction (in address PC).
          // 2. switch() over it to decide on a course of action.
          // 3. Do whatever the instruction should do according to the spec.
          // 4. Move the PC to the next instruction.
  // line 75 -- TODO: cpu.c Initialize the PC and other special registers

  // line 77 -- TODO: cpu.c Zero registers and RAM


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
  unsigned char PC = (int)cpu->PC;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
        unsigned char IR = cpu_ram_read(cpu, PC);
        unsigned char operandA = cpu_ram_read(cpu, (PC + 1));
        unsigned char operandB = cpu_ram_read(cpu, (PC + 2));

    // 2. switch() over it to decide on a course of action.

    switch(IR)
    {
      unsigned char *SP = &cpu->reg[7];
      case LDI:
        reg[operandA] = operandB;
        PC+=3;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        PC+=3;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        PC +=3;
        break;
      case DIV:
        alu(cpu, ALU_DIV, operandA, operandB);
        PC +=3;
        break;
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        PC +=3;
        break;
      case MOD:
        alu(cpu, ALU_MOD, operandA, operandB);
        PC +=3;
        break;
      case POP:
        reg[operandA] = cpu_ram_read(cpu, *SP++);
        PC +=2;
        break;
      case PUSH:
        cpu_ram_write(cpu, *--SP, reg[operandA]);
        PC +=2;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        PC+=2;
        break;
      case HLT:
        running = 0;
        break;
      default:
        fprintf(stderr, "Error setting instruction\n");
        exit(2);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
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

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
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
