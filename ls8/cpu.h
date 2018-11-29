#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int PC;
  int registers[8];
  int ram[256];
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
#define PRN	 0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file_name);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char loc);
extern void cpu_ram_write(struct cpu *cpu, unsigned char loc, int item);

#endif
