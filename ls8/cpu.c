#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * read from and write data to RAM
 */
unsigned char cpu_ram_read(struct cpu* cpu, unsigned char address)
{
    return cpu->ram[ address ];
}
 void cpu_ram_write(struct cpu* cpu, unsigned char address, unsigned char value)
{
    cpu->ram[ address ] = value;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{

  unsigned char address = 0;
  char line[256];
  FILE *fp;

  fp = fopen(filename, "r");

  if (fp == NULL)
  {
    printf(stderr, "The file you're looking for doesn't exist.");
    exit(3);
  }

  while (fgets(line, sizeof(line), fp) != NULL) {
    if (line[0] == '\n' || line[0] == '#') {
      continue;
    }
    unsigned char binary_number;
    binary_number = strtoul(line, NULL, 2);
    cpu->ram[address++] = binary_number;
  }

  fclose(fp);

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

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] *= cpu->reg[regB];
      // break;
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  
  int running = 1; // True until we get a HLT instruction
  unsigned char SP = cpu->reg[7];

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // unsigned char SP = cpu->registers[7];

    printf("TRACE: %02x: %02x %02x %02x\n", cpu->PC, IR, operandA, operandB);

    switch(IR)
    {
      case LDI:
        printf("Loading program...\n");
        cpu->reg[operandA] = operandB;
        cpu->PC += 3;
        break;
      case MUL:
        printf("Multiplying...\n");
        alu(cpu, ALU_MUL, operandA, operandB);
        // break; makes it hang at multiplying...
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC += 2;
        break;
      case PUSH: 
        printf("Pushing...\n");
        --cpu->reg[7];
        cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA]);
        cpu->PC += 2;
        break;
      case POP: 
        printf("Popping...\n");
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[7]);
        cpu->reg[7]++; 
        cpu->PC += 2;
        break;
      case HLT:
        printf("...Halting program\n");
        running = 0;
        break;
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
  cpu->reg[7] = 0xF4;
  // TODO: Zero registers and RAM
}
