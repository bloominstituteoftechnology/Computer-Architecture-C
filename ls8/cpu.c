#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
//  */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  
  while (running) {
  //   // TODO
  //   // 1. Get the value of the current instruction (in address PC).
     cpu->IR = cpu->ram[cpu->PC];
    unsigned char operandA = cpu->ram[cpu->PC + 1];
    unsigned char operandB = cpu->ram[cpu->PC + 2]; 

    printf("%d\n",operandB);

  //   // 2. switch() over it to decide on a course of action.
    switch (cpu->IR){
    case HLT:
      printf("Test\n");
      break;
    case LDI:
      printf("Test2\n");
      break;
    default:
    printf("test3\n");

    }
  //   // 3. Do whatever the instruction should do according to the spec.
  //   // 4. Move the PC to the next instruction.

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  cpu->PC = 0;
  cpu->FL = 0;
  cpu->registers =calloc(8,sizeof(unsigned char));
  cpu->ram = calloc(256,sizeof(unsigned char));
  // TODO: Zero registers and RAM
  
  cpu->registers[7] = 0xF4;
}

unsigned char cpu_ram_read(struct cpu *cpu)
{
  cpu->MDR =  cpu->ram[cpu->MAR];
  return cpu->MDR;
}

void cpu_ram_write(struct cpu *cpu)
{ 
    if(cpu->MAR < 0xF4){
      cpu->ram[cpu->MAR] = cpu->MDR;
    }   
}

