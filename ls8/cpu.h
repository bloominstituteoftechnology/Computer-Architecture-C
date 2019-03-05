#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // PC, 
  // registers (array) R0-R7.
  // ram (array)
  // The LS-8 has 8-bit addressing, so can address 256 bytes of RAM total.
  unsigned int PC;			// program counter
  unsigned char REG[8]; // 8-bit registers 2^8
  unsigned char ram[256];
};

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
