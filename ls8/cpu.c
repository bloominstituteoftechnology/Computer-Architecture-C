#include "cpu.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu,int index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu* cpu,unsigned char index,unsigned char value){
  cpu->ram[index]=value;
}
void setregister(struct cpu *cpu){
  cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]=cpu_ram_read(cpu,cpu->PC+2);
}
void print(struct cpu *cpu){
  printf("%hhu\n",cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]);
}
void halt(int *running){
  *running=0;
}
void multiply(struct cpu *cpu) {
  unsigned char a=cpu->registers[cpu_ram_read(cpu,cpu->PC+1)];
  unsigned char b=cpu->registers[cpu_ram_read(cpu,cpu->PC+2)];
  unsigned char ans=0;
  while (1) {
    if (b&1) {
      ans+=a;
      if (b==1) {
        cpu->registers[cpu_ram_read(cpu,cpu->PC+1)]=ans;
        break;
      }
    }
    a<<=1;
    b>>=1;
  }
}
void push(struct cpu *cpu) {
  cpu->registers[SP]-=1;
  cpu_ram_write(cpu,cpu->registers[SP],cpu->registers[cpu->ram[cpu->PC+1]]);
}
void pop(struct cpu *cpu) {
  cpu->registers[cpu->ram[cpu->PC+1]]=cpu_ram_read(cpu,cpu->registers[SP]);
  cpu->registers[SP]+=1;
}
void call(struct cpu *cpu) {
  cpu->registers[SP]-=1;
  cpu_ram_write(cpu,cpu->registers[SP],cpu->PC+2);
  cpu->PC=cpu->registers[cpu->ram[cpu->PC+1]];
}
void ret(struct cpu *cpu) {
  cpu->PC=cpu_ram_read(cpu,cpu->registers[SP]);
  cpu->registers[SP]+=1;
}
void add(struct cpu *cpu) {
  unsigned char ans=cpu->registers[cpu->ram[cpu->PC+1]]+cpu->registers[cpu->ram[cpu->PC+2]];
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void compare(struct cpu *cpu) {
  if (cpu->registers[cpu->ram[cpu->PC+1]]==cpu->registers[cpu->ram[cpu->PC+2]]) {
    cpu->equal=1;
  } else {
    cpu->equal=0;
  }
}
void jump(struct cpu *cpu) {
  cpu->PC=cpu->registers[cpu->ram[cpu->PC+1]];
}
void jump_equality(struct cpu *cpu) {
  if (cpu->equal==1) {
    jump(cpu);
  }
}
void jump_inequality(struct cpu *cpu) {
  if (cpu->equal==0) {
    jump(cpu);
  }
}
void cpu_load(struct cpu *cpu,char *filename)
{
  FILE *fp=fopen(filename,"r");
  char string[128];
  int address = 0;
  if (fp==NULL) {
    exit(1);
  }
  while (fgets(string,128,fp)!=NULL) {
    if (string[0]!='#') {
      unsigned char data=strtoul(string,NULL,2);
      cpu->ram[address++] = data;
    }
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
        setregister(cpu);
        break;
      case PRN:
        print(cpu);
        break;
      case MUL:
        multiply(cpu);
        break;
      case PUSH:
        push(cpu);
        break;
      case POP:
        pop(cpu);
        break;
      case HLT:
        halt(&running);
        break;
      case CALL:
        call(cpu);
        break;
      case RET:
        ret(cpu);
        break;
      case ADD:
        add(cpu);
        break;
      case CMP:
        compare(cpu);
        break;
      case JMP:
        jump(cpu);
        break;
      case JEQ:
        jump_equality(cpu);
        break;
      case JNE:
        jump_inequality(cpu);
        break;
    }
    if (instruction==cpu_ram_read(cpu,cpu->PC)) {
      cpu->PC+=((instruction>>6)+1);
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
  cpu->registers[SP]=0xF4;
  cpu->equal=0;
}
