#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char x;
  // registers (array)
  char *registers;
  // ram (array)
  char *ram;
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD,
  ALU_AND,
  ALU_CALL,
  ALU_CMP,
  ALU_DEC,
  ALU_DIV,
  ALU_HLT,
  ALU_INC,
  ALU_IRET,
  ALU_JEQ,
  ALU_JGE,
  ALU_JGT,
  ALU_JLE,
  ALU_JLT,
  ALU_JMP,
  ALU_JNE,
  ALU_LD,
  ALU_LDI,
  ALU_MOD,
  ALU_NOP,
  ALU_NOT,
  ALU_OR,
  ALU_POP,
  ALU_PRA,
  ALU_PRN,
  ALU_PUSH,
  ALU_RET,
  ALU_SHR,
  ALU_ST,
  ALU_SUB,
  ALU_XOR

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
