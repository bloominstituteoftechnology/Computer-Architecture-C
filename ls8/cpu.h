#ifndef _CPU_H_
#define _CPU_H_

struct cpu {
  unsigned char PC;
  unsigned char reg[8];
  unsigned char ram[8];
};

// ALU operations
enum alu_op {
  ALU_HLT,
  ALU_LDI,
  ALU_PRN,
  ALU_MUL
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hexliterals should be used.

#define HLT   0b00000001 
#define LDI   0b10000010
#define PRN   0b01000111
#define MUL   0b10100010

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
