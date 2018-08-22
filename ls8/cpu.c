#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  cpu->ram[MAR] = MDR;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
// void cpu_load(struct cpu *cpu, char *argv[])
// {
//   // file pointer
//   FILE *fp;
//   int address = 0;
//   char read[100];
//   printf("%sFILENAME\n", argv[1]);
//   fp = fopen(argv[1], "r");
//   if (fp == NULL)
//   {
//     fprintf(stderr, "Filepointer goofed\n");
//     exit(1);
//   }

//   // loop until fgets is at the end of the file
//   while (fgets(read, sizeof(read), fp))
//   {
//     unsigned long instruction;
//     char *endptr;
//     // cast line to binary until instruction is captured
//     instruction = strtoul(read, &endptr, 2);
//     // write instruction to RAM @address
//     if (!(*read == *endptr))
//       cpu_ram_write(cpu, address++, instruction);
//   }
// }
void cpu_load(struct cpu *cpu, char *argv[])
{

  // cpu->reg[7] = 0xF4; //cpu_load
  char line[100];
  int address = 0;
  FILE *fp = fopen(argv[1], "r");
  // printf("%s\n", filename);
  if (fp == NULL)
  {
    fprintf(stderr, "File pointer is  not working");
    exit(1);
  }
  while (fgets(line, sizeof(line), fp))
  {
    char *endchar;
    unsigned long v;
    v = strtoul(line, &endchar, 2);
    if (!(*line == *endchar))
      cpu_ram_write(cpu, address++, v);

    // {

    //   continue;
  }
  //     // printf("%u", v);
  //   }

  //   // const int DATA_LEN = 6;
  //   // char data[6] = {
  //   //     // From print8.ls8
  //   //     0b10000010, // LDI R0,8
  //   //     0b00000000,
  //   //     0b00001000,
  //   //     0b01000111, // PRN R0
  //   //     0b00000000,
  //   //     0b00000001 // HLT
  //   // };

  //   // int address = 0;

  //   // for (int i = 0; i < DATA_LEN; i++)
  //   // {
  //   //   cpu->ram[address++] = data[i];
  //   // }

  //   // TODO:
  //   // Replace this with something less hard - coded
  //   for (int i = 0; i < 5; i++)
  //   {
  //     printf("I am here in CPU_LOAD\n");
  //     printf("%02X: %02X\n", i, cpu->ram[i]);
  //   }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    break;

  case ALU_ADD:
    cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
    break;

  case ALU_DIV:
    cpu->reg[regA] = cpu->reg[regA] / cpu->reg[regB];
    break;

  case ALU_SUB:
    cpu->reg[regA] = cpu->reg[regA] - cpu->reg[regB];
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1;    // True until we get a HLT instruction
  cpu->reg[7] = 0xF4; //cpu_load

  while (running)
  {
    unsigned char IR = cpu_ram_read(cpu, cpu->pc);

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);

    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);

    printf("TRACE: %02x: %02x\n", cpu->pc, IR);
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // unsigned const char IR = ram->
    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB;
      // cpu->pc += 3;
      break;

    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;

    case HLT:
      running = 0;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

    case DIV:
      alu(cpu, ALU_DIV, operandA, operandB);
      break;

    case SUB:
      alu(cpu, ALU_SUB, operandA, operandB);
      break;

    case PUSH:
      // printf("I am in push");
      cpu->reg[7]--;
      cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA]);
      break;

    case POP:
      printf("I am in POP");
      // printf("%s POP\n", cpu->reg[operandA]);
      cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[7]);
      cpu->reg[7]++;
      break;

    default:
      printf("Unknown instruction at %02x: %02x\n", cpu->pc, IR);
      exit(2);
      // break;
    }
    // 3. Do whatever the instruction should do according to the spec.

    // 4. Move the PC to the next instruction.
    //PC + 1 or PC + 2
    cpu->pc += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // cpu->pc = 0;
  // TODO: Zero registers and RAM
  // for(int i = 0; i< si)
  cpu->reg[7] = 0xF4; //cpu_load
  memset(cpu->reg, 0, sizeof(cpu->reg));

  memset(cpu->ram, 0, sizeof(cpu->ram));
  // printf("I am here CPU_INIT \n");
}
