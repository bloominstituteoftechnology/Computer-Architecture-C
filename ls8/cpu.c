#include "cpu.h"
#include <string.h>
#include <stdio.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char ind)
{
  return cpu->ram[ind];
}
 void cpu_ram_write(struct cpu *cpu, unsigned char ind, unsigned char item)
{
  cpu->ram[ind] = item;
}

void stack_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = value;
}

unsigned char stack_pop(struct cpu *cpu)
{
  unsigned char retVal = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;
  return retVal;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path)
{
 FILE *fp;
 char line[8192];
 int address = 0;

 if((fp = fopen(path, "r")) == NULL){
   printf("Error opening file.\n");
   exit(2);
 }

 while(fgets(line, sizeof line, fp) != NULL) {
   char *endptr;
   unsigned char curr = strtol(line, &endptr, 2);

   if(endptr == line){
     continue;
   }

   cpu->ram[address++] = curr;
 }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
      case ALU_MUL:
        cpu->registers[regA] *= cpu->registers[regB];
        break;
      // TODO
      case ALU_CMP:
        if(cpu->registers[regA] > cpu->registers[regB]){
        cpu->FLAG = 00000010; 
        }else if(cpu->registers[regA] < cpu->registers[regB]){
        cpu->FLAG = 00000100; 
        }else{
        cpu->FLAG = 00000001; 
        }
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

unsigned char ir;
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    ir=cpu_ram_read(cpu, cpu->pc);
    unsigned char operandA=cpu_ram_read(cpu, cpu->pc + 1);
    unsigned char operandB=cpu_ram_read(cpu, cpu->pc + 2);
    // 2. switch() over it to decide on a course of action.
    switch(ir){
      case LDI:
        cpu->registers[operandA]= operandB;
        // cpu->pc+=3;
        break;
      case HLT:
        running=0;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        // cpu->pc+=2;
        break;
      case MUL:
        cpu->registers[operandA]*=cpu->registers[operandB];
        // cpu->pc+=3;
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB); 
          break; 
    }
    cpu->pc+=(ir >> 6) +1;
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
  cpu->pc=0;
  memset(cpu->registers, 0, 8);
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, 256);
}
