#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  //PC
  unsigned char PC;
  //registers (array)
  unsigned char reg[8];
  //ram (array)
  unsigned char ram[256];
  //flag
  unsigned char FL;
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_DIV,
  ALU_SUB,
  ALU_ADD,
  ALU_CMP
};

// Memory locations
#define PROGRAM_ENTRY 0x00
#define EMPTY_STACK 0xF4

// Flags
#define FLAG_EQ 0b00000001
#define FLAG_GT 0b00000010
#define FLAG_LT 0b00000100

#define ADD 0b10100000
#define CALL 0b01010000
#define HLT 0b00000001
#define LDI 0b10000010
#define MUL 0b10100010
#define POP 0b01000110
#define PRN 0b01000111
#define PUSH 0b01000101
#define RET 0b00010001
//Sprint Instructions
#define CMP 0b10100111
#define JMP 0b01010100
#define JEQ 0b01010101
#define JNE 0b01010110

// Function declarations

extern void cpu_load(char *filename, struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif