#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#define DATA_LEN 6

// #include "./examples/print8.ls8"
#define FILENAME "./examples/print8.ls8"
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu)
{
  // struct stat st;
  // stat(FILENAME, &st);
  // int strlen = st.st_size;

  // printf("strlen: %d\n", strlen);

  // FILE *file;
  // file = fopen(FILENAME, "r");
  // int c;
  // int index;

  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  // while (1)
  // {
  //   c = fgetint(file);
  //   if (feof(file))
  //   {
  //     break;
  //   }
  //   data[index] = c;
  //   index++;
  // }

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // fclose(file);
  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}


char cpu_ram_read(struct cpu *cpu, int i){
  return cpu->ram[i];
}

void cpu_ram_write(struct cpu *cpu, int i, unsigned char value){
  cpu->ram[i] = value;
  return 0;
}


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int reg;
  int reg_value;
  unsigned char value;
  int pc;

  while (running)
  {
    printf("Running\n");
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    pc = cpu->pc;
    value = cpu->ram[pc];
    printf("pc: %d\n", pc);
    printf("value: %d\n", value);
    // 2. Figure out how many operands this next instruction requires

    switch (value)
    {
    case LDI:
      reg = cpu->ram[pc + 1];
      printf("pc LDI1: %d\n", cpu->pc);
      printf("pc 1: %d\n", pc);
      reg_value = cpu->ram[pc + 2];
      printf("pc LDI2: %d\n", cpu->pc);
      printf("pc 2: %d\n", pc);
      cpu->registers[reg] = reg_value;
      printf("LDI\n");
      cpu->pc += 3;
      printf("pc LDI: %d\n", cpu->pc);
      break;
    case PRN:
      printf("PRN\n");
      reg = cpu->ram[pc + 1];
      reg_value = cpu->registers[reg];
      cpu->pc += 2;
      printf("Register Value: %d\n", reg_value);
      break;
    case HLT:
      running = 0;
      cpu->pc++;
      break;
    default:
      printf("Unknown instruction %02x at address %02x\n", value, pc);
      exit(1);
    }
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // struct cpu *cpu = malloc(sizeof(struct cpu));
  cpu->pc = 0;
  cpu->registers = malloc(8 * sizeof(unsigned char));
  cpu->ram = malloc(DATA_LEN * sizeof(unsigned char));
  return cpu;
}
