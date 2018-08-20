#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  int pc[8];
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010 //represents 8-bit value in binary literal
// TODO: more instructions here. These can be used in cpu_run().
#define ADD  10100000
#define SUB  10100001
#define MUL  10100010
#define DIV  10100011
#define MOD  10100100
#define INC  01100101
#define DEC  01100110
#define CMP  10100111

// Function declarations
extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
