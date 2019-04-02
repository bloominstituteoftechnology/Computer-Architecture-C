#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  // PC
  unsigned char pc;
  // FLAG
  unsigned char fl;
  // registers (array)
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  // Mathmatical operations
  ALU_MUL = 0b10100010,
  ALU_ADD = 0b10100000,
  ALU_SUB = 0b10100001,
  ALU_MOD = 0b10100100,
  ALU_DIV = 0b10100011,
  ALU_INC = 0b01100101,
  ALU_DEC = 0b01100110,
  ALU_CMP = 0b10100111,
  // Logic operations
  ALU_NOT = 0b01101001,
  ALU_AND = 0b10101000,
  ALU_OR = 0b10101010,
  ALU_XOR = 0b10101011,
  // Shifts operations
  ALU_SHL = 0b10101100,
  ALU_SHR = 0b10101101,
  ALU_ADDI = 0b11101000
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define POP 0b01000110
#define PUSH 0b01000101
#define CALL 0b01010000
#define RET 0b00010001
#define ST 0b10000100
#define JMP 0b01010100
#define PRA 0b01001000
#define IRET 0b00010011
#define JEQ 0b01010101
#define JNE 0b01010110

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
