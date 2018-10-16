#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(file, "r");
  if (fp == NULL) {
    printf("The file you are looking for doesn't exist\n");
  }
  while (fgets(line, sizeof(line), fp) != NULL) {
    // printf("%s", line);
    if (line[0] == '\n' || line[0] == '#') {
      continue;
    }

    unsigned char b;
    b = strtoul(line, NULL, 2);
    cpu->ram[address++] = b;
    // printf("%02X\n", b);
  }

  fclose(fp);

  // HARD CODED/OLD CODE
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
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value)
{
  cpu->ram[index] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
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

  while (running) {
    cpu->IR = cpu_ram_read(cpu, cpu->PC);
    int argc = cpu->IR >> 6;
    unsigned char argv[2];
    for (int i=0; i < argc; i++) {
      argv[i] = cpu_ram_read(cpu, cpu->PC + i + 1);
    }

    switch(cpu->IR) {
      case LDI:
        cpu->registers[argv[0]] = argv[1];
        break;
      case PRN:
        printf("%d\n", cpu->registers[argv[0]]);
        break;      
      case HLT:
        printf("Halting...\n");
        running = 0;
        break;      
      case MUL:
        alu(cpu, ALU_MUL, argv[0], argv[1]);
        break;      
      default:
        printf("Unknown instructions: %d", cpu->IR);
        break;
    }

    int changed = cpu->IR >> 4 & 1;
    if (!changed) {
      cpu->PC += 1 + argc;
    }
    
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // unsigned char IR = cpu_ram_read(cpu, PC);
    // 2. switch() over it to decide on a course of action.
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
  cpu->PC = 0;
  cpu->IR = 0;
  // TODO: Zero registers and RAM
  for (int i=0; i<8; i++) {
    cpu->registers[i] = 0;
  }
  for (int i=0; i<256; i++) {
    cpu->ram[i] = 0;
  }
}
