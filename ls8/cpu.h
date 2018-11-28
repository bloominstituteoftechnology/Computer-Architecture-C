#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int PC;
  int IR;
  int MAR;
  int MDL;
  int FL;
  unsigned char registers[8];
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL,
	ALU_ADD,
  ALU_SUB,
  ALU_DIV
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001 
#define LD   0b10000011  
#define ST   0b10000100  
#define PUSH 0b01000101 
#define POP  0b01000110 
#define PRN  0b01000111 
//ALU
#define ALU_ADD  0b10100000
#define ALU_SUB  0b10100001
#define ALU_MUL  0b10100010
#define ALU_DIV  0b10100011



// Function declarations

extern void cpu_load(struct cpu *cpu, int argc, char *arg);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
