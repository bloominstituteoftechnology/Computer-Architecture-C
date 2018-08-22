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
};

// Function declarations
extern void cpu_load(struct cpu *cpu, char *program);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
