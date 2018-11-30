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

  int value = cpu->registers[place] + cpu->registers[place2];
  cpu->registers[place] = value;
}
int convert(int dec)
{
  if (dec == 0)
  {
    return 0;
  }
  else
  {
    return (dec % 2 + 10 * convert(dec / 2));
  }
}

void update_flags(struct cpu *cpu, int compare_status)
{
  //0 is the E   1 is the L  and 2 is the G
  switch (compare_status)
  {

  case 0:
    cpu->FL[0] = 1; // 'E'
    cpu->FL[1] = 0; //'L'
    cpu->FL[2] = 0; // 'G'
    break;
  case 1:
    cpu->FL[0] = 0; // 'E'
    cpu->FL[1] = 0; //'L'
    cpu->FL[2] = 1; // 'G'
    break;
  case -1:
    cpu->FL[0] = 0; // 'E'
    cpu->FL[1] = 1; //'L'
    cpu->FL[2] = 0; // 'G'
    break;
  }
}

void compare(struct cpu *cpu, unsigned char place, unsigned char place2)
{
  int compare_status = -3; //set to -3 because I am only going to check for 1 0 -1.

  if (cpu->registers[place] == cpu->registers[place2])
  {
    compare_status = 0;
  }
  else if (cpu->registers[place] > cpu->registers[place2])
  {
    compare_status = 1;
  }
  else
  {
    //place is less than place2
    compare_status = -1;
  }

  update_flags(cpu, compare_status); // a function should server one purpose so I creaed another
  //update_flags will handle the conditionals and updating for the cpu->FL.
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
    cpu->registers[7] = cpu->ram[SP]; //meeting spec requirements register 7  has the pointer to the top of ram.
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
      SP--;
      cpu->ram[SP] = cpu->PC + 2;
      cpu->PC = cpu->registers[operandA];
      break;
    case RET:
      cpu->PC = cpu->ram[SP];
      // cpu->PC--;
      SP++;
      continue;

    case CMP:
      compare(cpu, operandA, operandB);
      cpu->PC += 2;
      break;

    case JMP:
      cpu->PC = cpu->registers[operandA];
      //May need to decrement possibly? check it out first. or continue instead of break
      break;

    case JEQ:
      if (cpu->FL[0] == 1)
      { //cpu->FL[0] is the 'E' flag.
        cpu->PC = cpu->registers[operandA];
      }
      else
      {
        cpu->PC += 1;
        //maybe 2  check it out first.
      }
      break;

    case JNE:
      if (cpu->FL[0] == 0)
      { //cpu->FL[0] is the 'E' flag.
        cpu->PC = cpu->registers[operandA];
        //May need to decrement possibly? check it out first. or continue instead of break
      }
      else
      {
        cpu->PC += 1;
        //maybe 2  check it out first.
      }
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
  //Flags
  memset(cpu->FL, 0, 3);
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, 256);     //RAM
  memset(cpu->registers, 0, 8); //registers
}
