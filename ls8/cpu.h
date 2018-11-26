#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC = 0;
  // registers (array) the number of wires
  unsigned char registers[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations - arithmetic logic unit, a digital circuit used to perform arithmetic and logic operations
enum alu_op {
  ALU_ADD,
  ALU_SUB,
	ALU_MUL,
  ALU_DIV
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010 //<-this is 130
// TODO: more instructions here. These can be used in cpu_run().



// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
