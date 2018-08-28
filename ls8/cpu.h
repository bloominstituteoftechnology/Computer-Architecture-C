#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC; // the type for a single unsigned byte in C
  // registers (array)
  unsigned char reg[8]; // we have to specify unsigned with a char otherwise the compiler will be the one to use which one to use
  // ram (array)
  unsigned char ram[256]; // 256 bytes for the RAM
};

// ALU operations
enum alu_op { // enum stands for enumerated type
  ALU_NUL,  // assigned an integer value and defaults to starting at 0
  ALU_ADD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define PRN 0b01000111
#define HLT 0b00000001 

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
