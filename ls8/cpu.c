#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"


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
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       cpu->register[regA] = regB; // just to use the values so that i don't get warnings
//       //will make changes to this later. 
//       break;

//     // TODO: implement more ALU ops
//   }
// }
// 
/*Helper functions step 2*/

void cpu_ram_read(struct cpu *cpu, unsigned char  place){
  printf("%d\n", cpu->registers[place] );  
}

void cpu_ram_write(struct cpu *cpu, unsigned char place, unsigned char saving){
  cpu->registers[place] = saving; 
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
    unsigned char  current = cpu->ram[cpu->PC]; 
    unsigned char operandA = cpu->ram[cpu->PC +1];
    unsigned char operandB  = cpu->ram[cpu->PC +2];
    switch (current){

      case LDI:
        cpu_ram_write(cpu, operandA, operandB); 
        cpu->PC += 2; 
        break; 
      case PRN:
        cpu_ram_read(cpu, operandA);
        cpu->PC += 1; 
        break; 
      case HLT:
        //
        running = 0; //should end loop
        break;    

    }
    cpu->PC += 1; 
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0; 
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, 256); //RAM 
  memset(cpu->registers, 0, 8); //registers
}


