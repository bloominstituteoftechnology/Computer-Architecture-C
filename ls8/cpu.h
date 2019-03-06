#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
typedef struct cpu {
  // TODO
  // PC
  unsigned char pc;
  unsigned char fl;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
} cpu;

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD
	// Add more here
};

// Instructions
//memory locations !!!!!!!!!!!!!!!!!
#define ADD_EMPTY_STACK 0xf4


// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().
#define MUL  0b10100010
#define ADD  0b10100000

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
