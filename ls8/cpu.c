#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

//from solution lecture
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
void cpu_load(struct cpu *cpu, char *argv[])
{

//const int DATA_LEN = 6;
//  char data[6] = { // I cannot get this to work with the variable
//    // From print8.ls8
//    0b10000010, // LDI R0,8
//    0b00000000,
//    0b00001000,
//    0b01000111, // PRN R0
//    0b00000000,
//    0b00000001  // HLT
//  };
  FILE *fp = fopen(argv[1], "r"); // https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm
  int address = 0;
  char buffer[256];
  char *pointer;
  // if file not found, error
  if (fp == NULL)
  {
    fprintf(stderr, "fail\n");
    exit(1);
  }
  while (fgets(buffer, sizeof(buffer), fp) != NULL)
  {
    cpu->ram[address++] = strtoul(buffer, &pointer, 2); // convert string to unsigned long integer, base 2
    // store string in the pointer
  }
//  for (int i = 0; i < DATA_LEN; i++) {
//    cpu->ram[address++] = data[i];
//  }

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
      cpu->registers[regA] *= cpu->registers[regB];
      break;
    // TODO: implement more ALU ops
    default:
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // instruction register IR holds the instruction currently being executed
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // some operations need the two bytes after PC to perform operations
    // so we're creating variables just in case
    switch (IR)
    {
        case MUL:
            alu(cpu, ALU_MUL, operandA, operandB);
            cpu->PC += 1;
            break;
        case HLT:
            running = 0;
            break;
        case PRN:
            printf("%d\n", cpu->registers[operandA]);
            cpu->PC += 2;
            break;
        case LDI:
            cpu->registers[operandA] = operandB;
            cpu->PC += 3;
            break;
        default:
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
  
  // TODO: Zero registers and RAM
}
