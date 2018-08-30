#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset comes from here
#include "cpu.h"

/* Helper Function
*  for inhenced readability and debugging
*    1. data validation 
*    2. data encapsulation
*/

// reading from the ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// write to the ram
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

// Push to Stack
void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SP]--;
  cpu->ram[cpu->reg[SP]] = val;
}

// Pop from Stack
unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;

  return val;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");

  char line[1024];

  // TODO: Replace this with something less hard-coded
  int address = ADDR_PROGRAM_ENTRY;

  if(fp == NULL)
  {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  //Read all the lines and store them in RAM

  while(fgets(line, sizeof line, fp) != NULL)
  {
    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);

    // ignore empty lines
    if(endchar == line)
    {
      continue;
    }

    // storing the bytes in our RAM
    cpu_ram_write(cpu, address++, byte);
    }
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
      break;
      // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
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
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    int instruction_set_pc = (IR >> 4) & 1;
    // 2. switch() over it to decide on a course of action.
    // what to do with the instructions?
    // 3. Do whatever the instruction should do according to the spec.
    switch(IR) {
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        // cpu->pc += 2;
        break;

      case HLT: 
        running = 0;
        break;

      case LDI:
        cpu->reg[operandA] = operandB;
        // cpu->pc += 3;
        break;
      
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu->reg[operandA] *= cpu->reg[operandB];
        // cpu->pc += 3; 
        break;

      case ADD:
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu->reg[operandA] += cpu->reg[operandB];
        // cpu->pc += 3;
        break;

      case PUSH:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
        break;

        // cpu_push(cpu, cpu->reg[operandA]);
        // break;

      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]);
        cpu->reg[SP]++;
        break;

        // cpu->reg[operandA] = cpu_pop(cpu);
        // break;

      
      default:
        printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
        exit(2); 
    }
    // 4. Move the PC to the next instruction.
      if (!instruction_set_pc)
        {
            cpu->pc += (IR >> 6) + 1;
        }

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->reg[SP] = 0xf4;

  // TODO: Zero registers and RAM

  // memset() is like fill() in JS
  memset(cpu->ram, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);

}

