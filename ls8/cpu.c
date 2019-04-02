#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

// read what is in the ram
// `MAR`: Memory Address Register, holds the memory address we're reading or writing
int cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
};

// write to the ram
// * `MDR`: Memory Data Register, holds the value to write or the value just read
void cpu_ram_write(struct cpu *cpu, unsigned char mdr)
{
  cpu->ram[mdr] = mdr;
};

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{

  // char data[DATA_LEN] = {
  //     // From print8.ls8
  //     0b10000010, // LDI R0,8
  //     0b00000000,
  //     0b00001000,
  //     0b01000111, // PRN R0
  //     0b00000000,
  //     0b00000001 // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++)
  // {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded

  // open the file
  FILE *fp = fopen(filename, "r");
  // check if the file is empty and return error
  if (fp == NULL)
  {
    fprintf(stderr, "ls8: error opening file:  %s\n", filename);
    exit(2);
  }

  char line[9999]; // hold lines
  int address = 0;
  // while fgets still has stuff
  while (fgets(line, sizeof(line), fp) != NULL)
  {
    char *endptr; // grabs none int lines
    // converts string to ints
    unsigned char val = strtoul(line, &endptr, 2);
    // prevents collecting none int lines
    if (line == endptr)
    {
      continue;
    }
    // store in memory
    cpu->ram[address++] = val;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB) // what are these?
{
  switch (op)
  {
  case ALU_MUL:
    //Multiply the values in two registers together and store the result in registerA.
    cpu->registers[regA] = regA * regB;
    break;
  case ALU_ADD:
    cpu->registers[regA] = regA + regB;
    break;
    // TODO: implement more ALU ops
  case ALU_NOP:
    // No operation. Do nothing for this instruction
    break;
  case ALU_NOT:
    break;
  case ALU_POP:
    // Pop the value at the top of the stack into the given register.
    // 1. Copy the value from the address pointed to by `SP` to the given register.2. Increment `SP`.
    break;
  case ALU_PRA:
    // `PRA register` pseudo - instruction
    //Print alpha character value stored in the given register.
    //Print to the console the ASCII character corresponding to the value in the register.
    break;
  case ALU_PRN:
    /* `PRN register` pseudo-instruction

    Print numeric value stored in the given register.
Print to the console the decimal integer value that is stored in the given register.

Machine code :
``` 01000111 00000rrr 47 0r
```*/
    break;
  case ALU_PUSH:
    /*`PUSH register`

Push the value in the given register on the stack.

1. Decrement the `SP`.
2. Copy the value in the given register to the address pointed to by
   `SP`.*/
    break;
  case ALU_RET:
    /*Pop the value from the top of the stack and store it in the `PC`.*/
    break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  // init current instructions
  unsigned char IR;
  // init operand 1 and 2
  unsigned char operand1;
  unsigned char operand2;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    operand1 = cpu_ram_read(cpu, cpu->PC + 1);
    operand2 = cpu_ram_read(cpu, cpu->PC + 2);
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    switch (IR)
    {
      // LDI
    case 0b10000010:
      // TODO: finish this
      // cpu->registers[operand1] =
      break;
      // PRN
    case 0b01000111:
      printf("%d", cpu->registers[operand1]);
      printf("%d", cpu->registers[operand2]);
      break;
      // HLT
    case 0b00000001:
      running = 0; // stops the while loop
      break;
      // TODO: if I have time to do it
      // MUL
    case 0b10100010:
      break;
    default:
      printf("Your code does not work bruhh %d", IR);
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // DONE✔:
  // Initialize the PC and other special registers
  // DONE✔: R0 - R6 are cleared to 0
  for (int i = 0; i < 6; i++)
  {
    // DONE✔: cpu->PC = '0';
    cpu->registers[i] = 0;
  }
  // DONE✔: R7 is set to 0xF4
  cpu->registers[7] = 0xF4;
  // DONE✔: PC is set to 0
  cpu->PC = 0;
  // DONE✔: RAM is set to 0
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
