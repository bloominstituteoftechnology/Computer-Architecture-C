#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned int PC;
  // registers (array)
  unsigned int SP;
  unsigned char ram[256];
  unsigned char reg[8];
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD,
  ALU_CMP,
  ALU_INC,
  ALU_DEC,
  // alu_add
  // alu_cmp
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().
#define ADD  0b10100000
#define PUSH 0b01000101
#define POP  0b01000110
#define MUL  0b10100010
#define DIV  0b10100011
#define INC  0b01100101
// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
