#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // PC
  unsigned int PC; // Program Counter

  //FL
  unsigned int FL; // Flags (00000LGE)

  // IS
  unsigned int IS; // Interrupt

  // registers (array)
  unsigned char reg[8];

  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	// Add more here
  ALU_ADD,
  ALU_AND,
  ALU_CMP,
  ALU_DEC,
  ALU_DIV,
  ALU_INC,
  ALU_MOD,
	ALU_MUL,
  ALU_NOT,
  ALU_OR,
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
// TODO: more instructions here. These can be used in cpu_run().

#define ADD  0b10100000
#define AND  0b10101000
#define CALL 0b01010000
#define CMP  0b10100111
#define DEC  0b01100110
#define DIV  0b10100011
#define INC  0b01100101
#define INT  0b01010010
#define IRET 0b00010011 // not done
#define JEQ  0b01010101
#define JGE  0b01011010
#define JGT  0b01010111
#define JLE  0b01011001
#define JLT  0b01011000
#define JMP  0b01010100
#define JNE  0b01010110
#define LD   0b10000011
#define LDI  0b10000010
#define HLT  0b00000001
#define MOD  0b10100100
#define MUL  0b10100010
#define NOP  0b00000000
#define NOT  0b01101001
#define OR   0b10101010
#define PRN  0b01000111

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
