#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;
  // registers (array)
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
  // stack pointer and array
  unsigned char SP;
};

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern void cpu_push(struct cpu *cpu, unsigned char val);
extern unsigned char cpu_pop(struct cpu *cpu);
extern void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr);
extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar);

#endif
