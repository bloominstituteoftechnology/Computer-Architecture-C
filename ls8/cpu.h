#ifndef _CPU_H_
#define _CPU_H_

struct cpu {
  // Internal Registers
  unsigned char pc;
  unsigned char ir;
  unsigned char mar;
  unsigned char mdr;
  unsigned char fl;

  // Registers
  unsigned char registers[8];

  // Memory
  unsigned char ram[256];

  // Stack
  unsigned char sp;
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
