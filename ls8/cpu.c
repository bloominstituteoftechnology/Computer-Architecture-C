#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{

  FILE *fp = fopen(filename, "r");
  char line[32];

  if (fp == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  int mem_index = 0;

  while (fgets(line, sizeof line, fp) != NULL)
  {
    cpu->ram[mem_index] = strtoul(line, NULL, 2);
    mem_index++;
  }

  fclose(fp);
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

void cpu_reg_read(struct cpu *cpu, unsigned char place)
{
  printf("%d\n", cpu->registers[place]);
}

void cpu_reg_write(struct cpu *cpu, unsigned char place, unsigned char saving)
{
  cpu->registers[place] = saving;
}
void cpu_reg_multiply(struct cpu *cpu, unsigned char place, unsigned char place2)
{
  cpu->registers[place] = cpu->registers[place] * cpu->registers[place2];
}
void cpu_reg_add(struct cpu *cpu, unsigned char place, unsigned char place2)
{
  printf("Are we adding\n");
  cpu->registers[place] = cpu->registers[place] + cpu->registers[place2];
  printf("yes we are adding here is the add data >> %d\n", cpu->registers[place]);
}

/**
 * Run the CPU
 */

void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int SP = 244;    //starting point of the stack;
  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char current = cpu->ram[cpu->PC];
    unsigned char operandA = cpu->ram[cpu->PC + 1];
    unsigned char operandB = cpu->ram[cpu->PC + 2];
    cpu->registers[7] = cpu->ram[SP];//meeting spec requirements register 7  has the pointer to the top of ram. 
    printf("%c\n", current);
    switch (current)
    {

    case LDI:
      cpu_reg_write(cpu, operandA, operandB);
      cpu->PC += 2;
      break;
    case PRN:
      cpu_reg_read(cpu, operandA);
      cpu->PC += 1;
      break;
    case MUL:
      cpu_reg_multiply(cpu, operandA, operandB);
      cpu->PC += 2;
      break;
    case ADD:
      cpu_reg_add(cpu, operandA, operandB);
      cpu->PC += 2; 
      break; 
    case PUSH:
      SP--;
      cpu->ram[SP] = cpu->registers[operandA];
      cpu->PC += 1;
      break;
    case POP:
      cpu_reg_write(cpu, operandA, cpu->ram[SP]);
      SP++;
      cpu->PC += 1;
      break;
    case CALL:
      //if 7 is the call instruction and 8 is HLT or any other instruction we take 8 with pc +1 push to stack. we want to get to 9.
      //The way I have it set up incrementing by 1 should be done here since pc is incremented after the switch. 
      // I push the current PC to the stack because when i get it in the return PC will be increased by 1 after the switch function.
      //This should place it where I want it to be. 
      SP--; 
      // cpu->ram[SP] = cpu->PC +1;
      cpu->ram[SP] = cpu->PC; 
      cpu->PC += 2; 
      printf("This should be next %d\n", cpu->ram[cpu->PC]);
      break;
    case RET:
      printf("The RET hits but before it hits things should have printed out though it does halt the program");
      cpu->PC = cpu->ram[SP];
      SP++; 
      break; 
    case HLT:
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
  memset(cpu->ram, 0, 256);     //RAM
  memset(cpu->registers, 0, 8); //registers
}
