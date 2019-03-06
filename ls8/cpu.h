#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC: Program Counter, address of the currently executing instruction
  unsigned int PC;
  // registers (array)
  unsigned char reg[8]; // R0-R7
  // ram (array)
  unsigned char ram[256]; // 0-255
  // flag
  unsigned char FL;

};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
// where programs start getting loaded
#define ADDR_PROGRAM_ENTRY 0x00
// where sp is on an empty stack
#define ADDR_EMPTY_STACK 0xF4
// these use binary literals.
#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
#define MUL  0b10100010

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_init(struct cpu *cpu);
extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
