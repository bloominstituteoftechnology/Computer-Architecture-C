#include <string.h>
#include <stdlib.h>
#include <stdio.h> //need for printf

#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  //open file
  FILE *fptr = fopen("", "r")

  //read line by line until returns null, ignore blank lines and comments
  while(fgets() != NULL)
  {
    
  }

  //save data into ram, have to convert binary strings to int values to store in ram with strtoul


  fclose(fptr);

  //original hard code we're refactoring
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

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
      break;

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

//mar is index in ram so set to new value 
void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] =  mdr;
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
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc+2);
    printf("TRACE: %0X2: %0X2  %0X2  %0X2 \n", cpu->pc, IR, operandA, operandB);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(IR)
    {
      case LDI:
        //sets value in given register to value specified 
        cpu->reg[operandA] = operandB;
        //increment by 3 since 3 arguments (CP+2)
        cpu->pc += 3;
        break;
      case PRN:
        //print out numerically in given register 
        printf("%d\n", cpu->reg[operandA]);
        cpu->pc += 2;
        break;
      case HLT:  //why is this needed when have break ? 
        running = 0;  //set running to false
        break;
      default:  //program stops if gets instruction it doesnt know
        printf("unexpected instruction: 0x%0X2 at 0x%0X2 \n", IR, cpu->pc);
        exit(1);
    }
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->reg, 0, sizeof(unsigned char)*8);  //just use size of cpu reg? 
  memset(cpu->ram, 0, sizeof(unsigned char)*256);
}
// When the LS-8 is booted, the following steps occur:

// * `R0`-`R6` are cleared to `0`.
// * `R7` is set to `0xF4`.
// * `PC` and `FL` registers are cleared to `0`.
// * RAM is cleared to `0`.