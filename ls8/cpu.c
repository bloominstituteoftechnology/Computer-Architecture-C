#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *dir)
{
  // open file
  FILE *fptr;

  if ((fptr = fopen(dir,"r"))== NULL){
    fprintf(stderr, "file does not exist: %s",dir);
    exit(1);
  }
  //
  unsigned char temp[256];
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

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value){
  cpu->ram[address] = value;
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
  unsigned char IR, operandA, operandB;
  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu,cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_ops = IR >> 6;
    
    // 3. Get the appropriate value(s) of the operands following this instruction
    switch (num_ops){
      case 1:
        operandA = cpu_ram_read(cpu,cpu->PC+1);
      case 2:
        operandA = cpu_ram_read(cpu,cpu->PC+1);
        operandB = cpu_ram_read(cpu,cpu->PC+2);
    }
    // 4. switch() over it to decide on a course of action.
    switch(IR){
    // 5. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu_ram_write(cpu,operandA,operandB);
      case PRN:
        printf("%d\n", cpu_ram_read(cpu,operandA));
      case HLT:
        return;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC = cpu->PC + num_ops + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  //Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->registers,0,8);
  memset(cpu->ram,0,256);
}
