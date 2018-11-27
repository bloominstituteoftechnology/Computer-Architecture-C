#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC,IR,MAR,MDR,FL;
  // registers (array)
  unsigned char *registers;
  // ram (array)
  unsigned char *ram;
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD,
  ALU_DIV,
  ALU_SUB
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001 
#define PRN  0b01000111
#define MUL  0b10100010
#define ADD  0b10100000
#define SUB  0b10100001
#define DIV  0b10100011
#define POP  0b01000110
#define PUSH 0b01000101
#define INC  0b01100101
#define DEC  0b01100110

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *fileName);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
