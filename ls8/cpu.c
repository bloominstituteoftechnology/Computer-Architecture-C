#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
};
  
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char new){
  //assign that index above to value
  return cpu->ram[index] = new;
};

void cpu_load(struct cpu *cpu, char *argv)
{
  printf("argv -> %s\n", argv);

  FILE * fp;
  fp = fopen(argv, "r");
  char str[30];
  // fp = fopen("./somefile.txt", "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);
  int address = 0;
  while( fgets(str, sizeof(str), fp) != NULL )
  {
      unsigned char binary;
      char *endptr;
      binary = strtoul(str, &endptr, 2);
      if (str != endptr) {
        // printf("%d", binary);
        cpu->ram[address++] = binary;
      }
  }
  printf("cpu at address 1: %d\n\n", cpu_ram_read(cpu, 0));

  fclose(fp);
  // exit(EXIT_SUCCESS);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    int IR = cpu->ram[cpu->PC];//this is the slot in ram

    unsigned int value1 = cpu_ram_read(cpu, cpu->PC+1); //the register index  where the second should be stored 
    unsigned int value2 = cpu_ram_read(cpu, cpu->PC+2);//the second number

    //value at register zero   

    // 1. Get the value of the current instruction (in address PC).
    switch(IR){
      case LDI:
        //set the value of a register to an integer
        cpu->reg[value1] = value2;
        cpu->PC+=3;
        break;
      case PRN:
        printf("The number printed is %u\n", cpu->reg[value1]);
        cpu->PC += 2;
        break;
      case HLT:
        printf("\nHalting Program.");
        exit(0); //terminates the whole program insetead of exiting switch... exit is kinda opposite ish 
        break;
      case MUL:
        // printf("product: %d\n", value1);
        alu(cpu, ALU_MUL, value1, value2);
        cpu->PC += 3;
        break;
      default: 
        exit(1);
    }
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram)); // 0 is what it is assigned to. 
  memset(cpu->reg, 0, sizeof(cpu->reg));
}