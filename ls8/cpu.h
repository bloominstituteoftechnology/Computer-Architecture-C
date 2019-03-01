#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO

  // PC
  unsigned char PC;

  // registers (array)
  unsigned char registers[8];

  // ram (array)
  unsigned char ram[256];
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

// General Instructions
#define LDI   0b10000010
#define HLT   0b00000001
#define PRN   0b01000111

// ALU Instructions
#define ADD   0b10100000
#define MUL   0b10100010

// Stack Instructions
#define POP   0b01000110
#define PUSH  0b01000101

// PC Explicit Instructions (assign PC directly)
#define CALL  0b01010000
#define INT   0b01010010
#define IRET  0b00010011
#define JMP   0b01010100
#define JNE   0b01010110
#define JEQ   0b01010101
#define JGT   0b01010111
#define JGE   0b01011010
#define JLT   0b01011000
#define JLE   0b01011001
#define RET   0b00010001

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *path);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
