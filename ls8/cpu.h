#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // PC is an array of index addresses
  unsigned char PC;
  unsigned char registers[8]; // registers are memory closest to my CPU
  unsigned char RAM[256];    // RAM is outside of the CPU
  int instruction_counter;
};
// PC = [0, 1, 2, 3, 4];
// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define PUSH 0b01000101
#define POP  0b01000110
#define LDI  0b10000010
#define MUL  0b10100010
#define PRN  0b01000111
#define HLT  0b00000001
#define CALL 0b01010000
#define RET  0b00010001
#define ADD  0b10100000
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif