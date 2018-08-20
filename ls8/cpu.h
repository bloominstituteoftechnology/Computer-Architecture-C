#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;
  // registers (array)
  unsigned char *registers;
  // ram (array)
  unsigned char *ram;
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define ALU_MUL  0b10100010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations
extern void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB);
extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
