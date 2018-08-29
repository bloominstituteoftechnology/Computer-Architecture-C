#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char pc;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
  ALU_MUL,
  ALU_ADD,
  ALU_LDI,
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
// TODO: more instructions here. These can be used in cpu_run().
#define HLT  0b10000001
#define PRN  0b01000111
#define MUL  0b10100010
#define ADD  0b10100000


// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);
extern void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value);

#endif
