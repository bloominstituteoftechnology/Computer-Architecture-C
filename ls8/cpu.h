#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  unsigned char PC;
  unsigned char reg[8];
  unsigned char ram[256];

};

// ALU operations
enum alu_op {
	ALU_MUL,
  // ALU_ADD = 0b10100000,
  // ALU_DEC = 0b01100110,
  // ALU_DIV = 0b10100011,
  // ALU_INC = 0b01100101,
  // ALU_MOD = 0b10100100,
  // ALU_SUB = 0b10100001,
  // ALU_CMP = 0b10100111,
  // ALU_AND = 0b10101000,
  // ALU_XOR = 0b10101011,
  // ALU_NOT = 0b01101001,
  // ALU_OR = 0b10101010,
  // ALU_SHL = 0b10101100,
  // ALU_SHR = 0b10101101
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define PRN 0b01000111
#define HLT 0b00000001
#define LDI 0b10000010
#define MUL 0b10100010

// enum non_alu_op {
//   NOP = 0b0000,
//   CALL= 0b0000,
//   HLT = 0b0001,
//   RET = 0b0001,
//   LDI = 0b0010,
//   INT = 0b0010,
//   IRET= 0b0011,
//   LD  = 0b0011,
//   JMP = 0b0100,
//   ST  = 0b0100
//   JEQ = 0b0101,
//   PUSH= 0b0101,
//   JNE = 0b0110,
//   POP = 0b0110,
//   JGT = 0b0111,
//   PRN = 0b0111,
//   JLT = 0b1000,
//   PRA = 0b1000,
//   JLE = 0b1001,
//   JGE = 0b1010,
// };

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, unsigned char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
#endif
