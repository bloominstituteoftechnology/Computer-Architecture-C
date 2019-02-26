#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

// struct cpu
// {
//   unsigned char pc;
//   unsigned char registry[8];
//   unsigned char ram[256];
// };

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value){
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
}


void cpu_load(struct cpu *cpu, char filename[])
{
  FILE *fptr;
  char *ptr;
  char buffer[255];
  char cmdString[9];
  
  unsigned int address = 0;
  
  fptr = fopen(filename ? filename : "examples/mult.ls8", "r");
  if(fptr){
   while(fgets(buffer,255,fptr)){
    if(buffer){
      //strncpy(cmdString,buffer,8);
      unsigned char cmd = strtoul(buffer, &ptr ,2);
      cpu->ram[address++] = cmd;    
    }        
  }
  } else {
    printf("Null");
  }
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

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  while (running) {
    unsigned char instruction = cpu_ram_read(cpu, cpu->pc);
    unsigned int operand = instruction >> 6;
    unsigned char operandA =  cpu_ram_read(cpu, cpu->pc+1);
    unsigned char operandB =  cpu_ram_read(cpu, cpu->pc+2);
   // printf("%s %d\n", "SP: ", cpu->SP);
    
    switch (instruction)
    {
      case HLT:
        running = 0;
        break;
      case LDI:
       // printf("%s %d %s %d\n", "LDI: Register",operandA, "equals" , operandB);
        cpu->registry[operandA] = operandB;
        break;
      case PRN:
        printf("%s %d %s %d\n", "PRN:",cpu->registry[operandA], ", Register:", operandA);
        break;
      case MUL:
        cpu->registry[operandA] *= cpu->registry[operandB];
        break;
      case PUSH:
        cpu->SP -= 1;
        //cpu_ram_write(cpu, cpu->SP, cpu->registry[operandA]);
        cpu->ram[cpu->SP] = cpu->registry[operandA];
        //printf("%s%d%s%d\n","PUSH: RAM Address ",cpu->SP," equals ",cpu->registry[operandA]);
        //printf("%s%d%s%d\n", "TESTING: RAM ADDRESS ", cpu->SP, " equals ", cpu->ram[cpu->SP] );
        
        
        break;
      case POP:        
        cpu->registry[operandA] = cpu_ram_read(cpu, cpu->SP++);
        
       // printf("%s%d%s%d\n","POP: RAM Address ",cpu->SP-1," equals ",cpu->registry[operandA]);
        //printf("%s%d%s%d\n", "TESTING: RAM ADDRESS ", cpu->SP-1, " equals ", cpu->ram[cpu->SP-1] );
        break;


    
      default:
        break;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  cpu->pc += 1 + operand;
  }
  
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registry,0,sizeof cpu->registry);
  cpu->pc = 0;
  cpu->SP = 0xf4;
}
