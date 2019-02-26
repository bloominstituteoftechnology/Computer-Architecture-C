#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
typedef struct cpu {
  // TODO
  // PC
  unsigned int pc;
  // registers (array)
  unsigned char reg[8]; // there are 8 total: R0-R7
  // ram (array)
  unsigned char ram[256];  // total amount of storage for this project
} CPU;

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
