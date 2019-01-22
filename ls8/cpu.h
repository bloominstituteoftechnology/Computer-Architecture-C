#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;//Program Counter, address of the currantly executing instruction
  unsigned char registers[8];//the register is made up 8 bits
  unsigned char ram[256];//The CPU could support 256 instructions.
  unsigned char SP;//The SP points at the value at the top of the stack (most recently pushed), or at
//address `F4` if the stack is empty.
  // registers (array)
  // ram (array)
  // if not signed or unsigned with a char, it's up to the compiler which it uses
  //
};

// ALU operations
enum alu_op {
	ALU_MUL,
  ALU_ADD
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
