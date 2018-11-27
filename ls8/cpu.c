#include "cpu.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu,int index) {
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, int index,unsigned char value) {
  cpu->ram[index]=value;
}
void cpu_load(struct cpu *cpu,char *filename)
{
  FILE *fp=fopen(filename,"r");
  if (fp==NULL) {
    printf("Error opening file.");
  }
  int address=0;
  char string[256];
  while (fgets(string,sizeof(string),fp)!=NULL) {
    unsigned char data=strtol(string,NULL,2);
    cpu->ram[address++]=data;
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char instruction=cpu_ram_read(cpu,cpu->PC);
    switch(instruction) {
      case LDI:
        cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]=cpu_ram_read(cpu,cpu->PC+2);
        cpu->PC+=3;
      case PRN:
        printf("%i\n",cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]);
        cpu->PC+=2;
      case HLT:
        running=0;
        cpu->PC+=1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // TODO: Zero registers and RAM
  cpu->PC=0;
  memset(cpu->registers,0,sizeof(cpu->registers));
  memset(cpu->ram,0,sizeof(cpu->ram));
}
