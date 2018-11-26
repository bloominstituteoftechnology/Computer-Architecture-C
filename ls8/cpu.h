#ifndef _CPU_H_
#define _CPU_H_

#define MAX_ADDRESS 0xff

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;

  // registers (array)
  unsigned char reg[8];

  // ram (array)
  unsigned char ram[MAX_ADDRESS + 1];
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
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
