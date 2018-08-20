#ifndef _CPU_H_ 
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  unsigned char PC; // PC
  unsigned char reg[8]; // registers (array)
  unsigned char ram[256]; // ram (array)
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations aka Function Prototypes aka forward declarations aka forward references
  // it's like declaring a varibale before assigning it a value; This will prevent 
  // compiler from complaining when it sees the functions being called in ls8.c
extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
