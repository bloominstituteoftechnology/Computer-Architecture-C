#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6


unsigned char cpu_ram_read(struct cpu *cpu)
{
  cpu->MDR =  cpu->ram[cpu->MAR];
  return cpu->MDR;
}

void cpu_ram_write(struct cpu *cpu)
{ 
    if(cpu->MAR < 0xF4){
      cpu->ram[cpu->MAR] = cpu->MDR;
    }   
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *fileName)
{
  FILE *fp;
  fp = fopen(fileName, "r"); // read mode
 
   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
  // char data = calloc(8, sizeof(char));
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  int address = 0;
  
  char s[1024];

  while(!feof(fp))
{
  fgets(s, sizeof(s), fp); 
      char *split=" ";

unsigned char binaryencoded = strtol(s, &split, 2);;
    if (split == s) {
      continue;
    }

    cpu->ram[address++] = binaryencoded;
}
   
   
  fclose(fp);

}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;
     case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;
    case ALU_DIV:
      cpu->registers[regA] = cpu->registers[regA] / cpu->registers[regB];
      break;
    case ALU_SUB:
      cpu->registers[regA] = cpu->registers[regA] - cpu->registers[regB];
      break;
    default:
      printf("Something broke in the alu function");
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
  //   // TODO
  //   // 1. Get the value of the current instruction (in address PC).
      cpu->MAR = cpu->PC;
     cpu->IR = cpu_ram_read(cpu);
    unsigned char operandA = cpu->ram[cpu->PC + 1];
    unsigned char operandB = cpu->ram[cpu->PC + 2]; 
    // printf("%d \n",cpu->PC);
  //   // 2. switch() over it to decide on a course of action.
    switch (cpu->IR){
    case HLT:
      running = 0;
      break;
    case LDI:
      cpu->registers[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n",cpu->registers[operandA]);
      break;
    case MUL:
      alu(cpu,ALU_MUL,operandA,operandB);
      break;
    case ADD:
      alu(cpu,ALU_ADD,operandA,operandB);
      break;
    case DIV:
      alu(cpu,ALU_DIV,operandA,operandB);
      break;
    case SUB:
      alu(cpu,ALU_SUB,operandA,operandB);
      break;
    default:
    printf("Something went wrong\n");

    }


  //   // 3. Do whatever the instruction should do according to the spec.
  //   // 4. Move the PC to the next instruction.
  if(cpu->IR >= 128){
      cpu->PC +=3;
  }
  else if(cpu->IR >= 64){
    cpu->PC +=2;
  }
  else{
    cpu->PC +=1;
  }
    
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  cpu->PC = 0;
  cpu->FL = 0;
  cpu->registers =calloc(8,sizeof(unsigned char));
  cpu->ram = calloc(256,sizeof(unsigned char));
  // TODO: Zero registers and RAM
  
  cpu->registers[7] = 0xF4;
}



