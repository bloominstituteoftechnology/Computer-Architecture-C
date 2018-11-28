#include "cpu.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu,int index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu,int index,unsigned char value){
  cpu->ram[index]=value;
}
void setregister(struct cpu *cpu,unsigned char instruction){
  cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]=cpu_ram_read(cpu,cpu->PC+2);
  cpu->PC+=((instruction>>6)+1);
}
void print(struct cpu *cpu,unsigned char instruction){
  printf("%i\n",cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]);
  cpu->PC+=((instruction>>6)+1);
}
void halt(struct cpu *cpu,unsigned char instruction,int *running){
  *running=0;
  cpu->PC+=((instruction>>6)+1);
}
void multiply(struct cpu *cpu,unsigned char instruction) {
  unsigned char a=cpu->registers[cpu_ram_read(cpu,cpu->PC+1)];
  unsigned char b=cpu->registers[cpu_ram_read(cpu,cpu->PC+2)];
  unsigned char ans=0;
  while (b>0) {
    if (b&1) {
      ans+=a;
    }
    a=a<<1;
    b=b>>1;
  }
  cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]=ans;
  cpu->PC+=((instruction>>6)+1);
}
void cpu_load(struct cpu *cpu,char *filename)
{
  FILE *fp=fopen(filename,"r");
  char string[128];
  int address = 0;

  while (fgets(string,128,fp)!=NULL) {
    unsigned char data=strtoul(string,NULL,2);
    cpu->ram[address++] = data;
  }
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char instruction=cpu_ram_read(cpu,cpu->PC);
    switch(instruction){
      case LDI:
        setregister(cpu,instruction);
        break;
      case PRN:
        print(cpu,instruction);
        break;
      case MUL:
        multiply(cpu,instruction);
        break;
      case HLT:
        halt(cpu,instruction,&running);
        break;
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
  memset(cpu->ram,0,sizeof(cpu->ram));
  memset(cpu->registers,0,sizeof(cpu->registers));
}
