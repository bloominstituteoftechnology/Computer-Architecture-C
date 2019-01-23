#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC --> keeps track of where we are in the program
  unsigned int PC; // --> Move me after each function is done
  // registers (array) --> where we push operand values
  unsigned char registers[8]; // --> R0 -> R7
  // ram (array) --> holds everything else 
  unsigned char ram[256]; // --> 11111111

};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_HTL,
  ALU_LDT,
  ALU_PRN,

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
