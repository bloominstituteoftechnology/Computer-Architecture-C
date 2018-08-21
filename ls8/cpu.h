#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int *ram[256];
  // TODO
  // PC

  unsigned char pc;
  unsigned char reg[8];
  unsigned char RAM[256];
  // registers (array)
  // ram (array)
} cpu;


// ALU operations
enum alu_op {
  ALU_MUL,
  ALU_ADD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
