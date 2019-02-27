#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char * argv[])
{
  FILE *fp;
  char *location = argv[1];
  char temp[256];
  fp = fopen(location, "r");
  int counter = 0;
  if(fp == NULL) {
      perror("Error opening file");
      exit(1);
  } else {
    while(fgets(temp, sizeof(temp), fp) != NULL) {
      unsigned char data = strtol(temp, NULL, 2);
      cpu_ram_write(cpu, data, counter);
      counter += 1;
    }

    
  }
  

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  if(cpu && regA && regB){
    
  }
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = (int)cpu->registers[regA] * (int)cpu->registers[regB];
      
      break;

    // TODO: implement more ALU ops
  }
}
unsigned char cpu_ram_read(struct cpu *cpu, int index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, char data, int index) {
  cpu->ram[index] = data;
}

int handle_LDI(struct cpu *cpu, unsigned char operandA, unsigned char operandB) {
  cpu->registers[operandA] = operandB;
  printf("%i INSIDE LDI\n", cpu->registers[operandA]);
  return 0;
}

int handle_PRN(struct cpu *cpu, unsigned char operandA, unsigned char operandB) {
  printf("Numeric Value = %i\n", cpu->registers[operandA]);
  return 0;
}

int handle_HLT(struct cpu *cpu, unsigned char operandA, unsigned char operandB) {
  return 0;
}

int handle_PUSH(struct cpu *cpu, unsigned char operandA) {
  cpu->registers[7]--;
  *cpu->registers[7] = cpu->registers[operandA];
}

int handle_POP(struct cpu *cpu, unsigned char operandA) {
  cpu->registers[operandA] = *cpu->registers[7];
  cpu->registers[7]++;
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
    
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    unsigned int numop = IR >> 6;

    switch(IR) {
      case(LDI):
        handle_LDI(cpu, operandA, operandB);
        break;
      case(PRN):
        handle_PRN(cpu, operandA, operandB);
        break;
      case(HLT):
        running = handle_HLT(cpu, operandA, operandB);
        break;
      case(MUL):
        alu(cpu, ALU_MUL,operandA, operandB);
        break;
      case(PUSH):
        handle_PUSH(cpu, operandA);
        break;
      case(POP):
        handle_POP(cpu, operandA);
        break;
    }
    
    cpu->PC += numop + 1;
    
    
    // 2. Figure out how many operands this next instruction requires
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
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->registers[7] = &cpu->ram[255];
  // TODO: Initialize the PC and other special registers
}

