#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD,
  LOAD_VALUE,
  NOTHING
	// Add more here
};

#define ADDR_PROGRAM_ENTRY 0x00 // Load
#define ADD_EMPTY_STACK 0xF4 // Empty Stack

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
