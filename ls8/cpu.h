#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // STEP 1 TODO
  // PC
  unsigned char PC;  // program counter: address of the currently executing instruction
  unsigned char IR;  // instruction register: contains a copy of the currently executing instruction
  unsigned char MAR; // Memory Address Register: holds the memory address we're reading/writing
  unsigned char MDR; // Memory Data Register: holds the value to write or the value we just read
  unsigned char FL;  // Flags: holds current status 00000LGE

  // registers (array)
  unsigned char registers[8]; // registers R0-R7
  // ram (array)
  unsigned char ram[256];
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

// TODO: more instructions here. These can be used in cpu_run().

// ALU

// PC Mutators

// Other
#define LDI 0b10000010
#define HLT 00000001
// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
