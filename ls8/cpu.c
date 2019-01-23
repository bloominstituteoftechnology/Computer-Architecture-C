#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_name)
{
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
//open file
FILE *fp=fopen(file_name,"r");
if (fp==NULL) {
  printf("Error: file unable to open.");
}
  // RAM address
  int address=0;
  char string[1024];

  //will read each line of file
  while (fgets(string,sizeof(string),fp)!=NULL) 
  {
    unsigned char data=strtol(string,NULL,2);

    // as instructions are added to RAM, RAM address increments
    cpu->ram[address++]=data;
   } 
  fclose(fp);


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

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char curr_instr = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    unsigned int next_operands = curr_instr >> 6;  

    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1); 
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);


    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.

    switch(curr_instr)
    {
      case LDI:
        cpu->registeres[operandA] = operandB; 
        break;
      
       case PRN:
        printf("%d\n", cpu->registeres[operandA]);
        break;

       case HLT:
        running = 0;
        break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += curr_instr + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC=0;
  memset(cpu->registeres, 0, sizeof(cpu->registeres));
  memset(cpu->ram, 0, sizeof(cpu->ram));
//   for (int i=0; i<8; i++) {
//     cpu->registeres[i] = 0;
//   }
//   for (int i=0;i<256; i++) {
//     cpu->ram[i] = 0;
//   }
}
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value){
  cpu->ram[index] = value;
} 