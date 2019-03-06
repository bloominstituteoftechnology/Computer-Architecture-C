#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *dir)
{
  FILE *fptr;

  if ((fptr = fopen(dir,"r"))== NULL)
  {
    fprintf(stderr, "File does not exist: %s\n",dir);
    exit(1);
  }
  //
  char temp[256];
  char *instruction;

  unsigned int counter = 0;

  while(fgets(temp,sizeof(temp),fptr) != NULL){
    instruction = strndup(temp,8);

    if (instruction[0] == '0' || instruction[0] == '1'){
      cpu->ram[counter] = strtoul(instruction,NULL,2);
      counter++;
    }
  }

  fclose(fptr);

}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address){
  cpu->ram[address] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  int num1,num2;
  switch (op) {
    case ALU_MUL:
      // TODO
      num1 = cpu_ram_read(cpu, regA);
      num2 = cpu_ram_read(cpu, regB);
      cpu_ram_write(cpu,regA,num1*num2);
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
  // Remember Zero is interpreted as false and anything non-zero is interpreted as true.

  unsigned char IR;
  unsigned char opA;
  unsigned char opB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    int operands = IR >> 6; 
    // 3. Get the appropriate value(s) of the operands following this instruction
    switch (operands){
      case 1:
        opA = cpu_ram_read(cpu,cpu->PC+1);
      case 2:
        opA = cpu_ram_read(cpu,cpu->PC+1);
        opB = cpu_ram_read(cpu,cpu->PC+2);
    }
    // 4. switch() over it (THE INSTRUCTION) to decide on a course of action.
    switch(IR){
    // 5. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu_ram_write(cpu, opA, opB);
        break;
      case PRN:
        printf("%d\n", cpu_ram_read(cpu,opA));
        break;
      case MUL:
        alu(cpu,ALU_MUL,opA,opB);
        break;
      case HLT:
      running = 0;
      break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC = cpu->PC + operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
   // Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->registers,0,8);
  memset(cpu->ram,0,256);
  
}


