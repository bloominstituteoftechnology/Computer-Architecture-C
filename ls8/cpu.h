#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU

struct cpu {
  // TODO
  // PC
  char pc[266];
  // registers (array)
  char reg[8];
  // ram (array)
  char ram[266];
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
// TODO: more instructions here. These can be used in cpu_run().
// ALU Ops
#define ADD  10100000
#define SUB  10100001
#define MUL  10100010
#define DIV  10100011
#define MOD  10100100
#define INC  01100101
#define DEC  01100110
#define CMP  10100111
#define AND  10101000
#define NOT  01101001
#define OR   10101010
#define XOR  10101011
#define SHL  10101100
#define SHR  10101101

// PC mutators
#define CALL 01010000
#define RET  00010001
#define INT  01010010
#define IRET 00010011
#define JMP  01010100
#define JEQ  01010101
#define JNE  01010110
#define JGT  01010111
#define JLT  01011000
#define JLE  01011001
#define JGE  01011010

//Others
#define NOP  00000000
#define HLT  00000001
#define LDI  10000010
#define LD   10000011
#define ST   10000100
#define PUSH 01000101
#define POP  01000110
#define PRN  01000111
#define PRA  01001000

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
