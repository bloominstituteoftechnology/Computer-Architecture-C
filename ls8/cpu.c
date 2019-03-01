#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6

/**
 * Read data from RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  //To Do: implement function to read from ram
  //instruction = cpu->pc;

  return cpu->ram[index];

  //should not be void because it is returning something. should this return a pointer to a byte of memory?
}

/**
 * Write data to RAM
 */
void cpu_write_ram(struct cpu *cpu, unsigned char index, unsigned char value)
{
  //To Do: implement function to write to ram
  //printf("%d\n", value);
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{

  // set R7 to store the start of the RAM

  char buffer[256];
  int index = 0;
  FILE *f = fopen(file, "r");
  while (fgets(buffer, sizeof(buffer), f) != NULL)
  {
    char *ptr;
    long instruction = strtol(buffer, &ptr, 2);
    if (ptr == buffer)
    {
      continue;
    }
    cpu_write_ram(cpu, index, instruction);
    index++;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
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

  while (running)
  {
    unsigned char operandA;
    unsigned char operandB;
    unsigned char next_instruction;
    unsigned int tmp;
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->pc];
    // unsigned char operandA = cpu_read_ram(cpu, (IR + 1));
    // unsigned char operandB = cpu_read_ram(cpu, IR + 2);

    // 2. Figure out how many operands this next instruction requires
    unsigned char masking_bit = 0b11000000;
    int number_of_operands = IR & masking_bit;
    number_of_operands = number_of_operands >> 6;
    //printf("\n----\nIR: %d\n", IR);
    // printf("masking bit: %d\n", masking_bit);
    // printf("number of operands: %02x\n", number_of_operands);
    // printf("LDI: %d\n", LDI);
    // printf("PRN: %d\n", PRN);
    // printf("HLT: %d\n", HLT);
    // printf("MUL : %d\n", MUL);
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (number_of_operands == 0)
    {
      // no operands!
    }

    if (number_of_operands == 1)
    {
      operandA = cpu_ram_read(cpu, cpu->pc + 1);
    }
    if (number_of_operands == 2)
    {
      operandA = cpu_ram_read(cpu, cpu->pc + 1);
      operandB = cpu_ram_read(cpu, cpu->pc + 2);
    }
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
    // 5. Do whatever the instruction should do according to the spec.
    case LDI:
      //printf("Let's load %d into register %d\n", operandB, operandA);
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      //printf("Lets print %d\n", operandB);
      printf("%d\n", cpu->registers[operandA]);
      break;
    case HLT:
      running = 0;
      break;

    case MUL:
      // to do: call ALU function
      //printf("Lets print multiplication of: %d\n", cpu->registers[operandA], cpu->registers[operandB]);
      //printf("%d\n", cpu->registers[operandA] * cpu->registers[operandB]);

      //cpu->registers[operandA] = cpu->registers[operandA] * cpu->registers[operandB];
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case PUSH:
      // decrement the SP (stack pointer)
      cpu->registers[7]--;
      //copy the value in given register to the addresss pointed to by SP
      cpu_write_ram(cpu, cpu->registers[7], cpu->registers[operandA]);
      break;
    case POP:
      //copy the value from the address pointed to by SP to the given register
      cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);
      //increment SP
      cpu->registers[7]++;
      break;
    case CALL:
      printf("lets CALL \n");
      //add the address of the instruction directly after the call to the stack
      //PUSH(cpu_read_ram(cpu, cpu->pc+number_of_operands+1)
      next_instruction = cpu_ram_read(cpu, cpu->pc + number_of_operands + 1);
      printf("next instruction: %d\n", next_instruction);
      cpu->registers[7]--;
      //cpu_write_ram(cpu, cpu->registers[7], next_instruction);
      cpu_write_ram(cpu, cpu->registers[7], cpu->pc);
      //PC is set to the address stored in the given register
      cpu->pc = cpu->registers[operandA];

      break;
    case RET:
      printf("i just want to RETurn");
      //POP the value from top of the stack:
      //copy the value from the address pointed to by SP to the given register
      cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[7]);

      //store it in PC:
      cpu->pc = cpu->registers[operandA];

      //increment SP
      cpu->registers[7]++;
      break;
    default:
      printf("something went wrong with IR : %d\n", IR);
      exit(1);
    }

    // 6. Move the PC to the next instruction.
    for (int i = 0; i <= number_of_operands; i++)
    {
      cpu->pc++;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  //memset(cpu->pc, 0, 1); -> is this needed?
  cpu->pc = 0;
  memset(cpu->registers, 0, 1);
  memset(cpu->ram, 0, sizeof(char));
  cpu->registers[7] = 0xf4;
}
