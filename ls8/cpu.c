#include "cpu.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define SP 7
#define e_true 0b00000001
#define e_false 0b00000000
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
void multiply(struct cpu *cpu,unsigned char a,unsigned char b) {
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
void add(struct cpu *cpu,unsigned char a, unsigned char b) {
  unsigned char ans=a+b;
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void compare(struct cpu *cpu,unsigned char a, unsigned char b) {
  if (a==b) {
    cpu->FL=e_true;
  } else {
    cpu->FL=e_false;
  }
}
void jump(struct cpu *cpu) {
  cpu->PC=cpu->registers[cpu->ram[cpu->PC+1]];
}
void jump_equality(struct cpu *cpu) {
  if (cpu->FL==e_true) {
    jump(cpu);
  }
}
void jump_inequality(struct cpu *cpu) {
  if (cpu->FL==e_false) {
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
    if (string[0]=='1' || string[0]=='0') {
      unsigned char data=strtoul(string,NULL,2);
      cpu->ram[address++] = data;
    }
  }
  // TODO: Replace this with something less hard-coded
}
void and(struct cpu *cpu, unsigned char a, unsigned char b) {
  unsigned char ans=a&b;
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void or(struct cpu *cpu,unsigned char a, unsigned char b) {
  unsigned char ans=a|b;
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void xor(struct cpu *cpu,unsigned char a, unsigned char b) {
  unsigned char ans=a^b;
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void not(struct cpu *cpu,unsigned char a) {
  unsigned char ans=~a;
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void mod(struct cpu *cpu,unsigned char a,unsigned char b,int *running) {
  if (b==0) {
    printf("In JavaScript the return value would be NaN.  Here your program crashes.\n");
    halt(running);
  } else {
    unsigned char ans=a%b;
    cpu->registers[cpu->ram[cpu->PC+1]]=ans;
  }
}
void shift_left(struct cpu *cpu,unsigned char a,unsigned char b) {
  unsigned char ans=a<<b;
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void shift_right(struct cpu *cpu,unsigned char a,unsigned char b) {
  unsigned char ans=a>>b;
  cpu->registers[cpu->ram[cpu->PC+1]]=ans;
}
void store(struct cpu *cpu) {
  cpu->ram[cpu->registers[cpu->ram[cpu->PC+1]]]=cpu->registers[cpu->ram[cpu->PC+2]];
}
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB,int *running)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      multiply(cpu,regA,regB);
      break;
    case ALU_ADD:
      add(cpu,regA,regB);
      break;
    case ALU_AND:
      and(cpu,regA,regB);
      break;
    case ALU_OR:
      or(cpu,regA,regB);
      break;
    case ALU_XOR:
      xor(cpu,regA,regB);
      break;
    case ALU_NOT:
      not(cpu,regA);
      break;
    case ALU_MOD:
      mod(cpu,regA,regB,running);
      break;
    case ALU_SHL:
      shift_left(cpu,regA,regB);
      break;
    case ALU_SHR:
      shift_right(cpu,regA,regB);
      break;
    case ALU_CMP:
      compare(cpu,regA,regB);
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
        alu(cpu,ALU_MUL,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
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
        alu(cpu,ALU_ADD,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
        break;
      case CMP:
        alu(cpu,ALU_CMP,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
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
      case AND:
        alu(cpu,ALU_AND,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
        break;
      case OR:
        alu(cpu,ALU_OR,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
        break;
      case XOR:
        alu(cpu,ALU_XOR,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
        break;
      case NOT:
        alu(cpu,ALU_NOT,cpu->registers[cpu->ram[cpu->PC+1]],0,0);
        break;
      case MOD:
        alu(cpu,ALU_MOD,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],&running);
        break;
      case SHL:
        alu(cpu,ALU_SHL,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
        break;
      case SHR:
        alu(cpu,ALU_SHR,cpu->registers[cpu->ram[cpu->PC+1]],cpu->registers[cpu->ram[cpu->PC+2]],0);
        break;
      case ST:
        store(cpu);
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
  cpu->FL=e_false;
}
