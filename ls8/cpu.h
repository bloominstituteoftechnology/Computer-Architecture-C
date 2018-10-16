#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {

  // PC
  unsigned char pc;

  // registers (array)
  unsigned char registers[8];  // 8 registers
  
  // ram (array)
  unsigned char ram[256]; // 256 bytes

};

// ALU operations
enum alu_op {
	ALU_MUL,
  // Add more here
  ALU_ADD
	
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
