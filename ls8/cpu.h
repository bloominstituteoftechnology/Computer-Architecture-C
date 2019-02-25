#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
      // TODO
      // The type for a single unsigned byte in C is : (unsigned char x;)

      // PC = program counter...address of current instruction
      unsigned char PC;
      
      // registers (array)
      /* 8 registers
          - R5 is reserved for interrupt mask
          - R6 is reserved for interrupt status
          - R7 is reserved for stack pointer
      */
      unsigned char reg[8];

      // ram (array)
      unsigned char ram[256]; //8-bit cpu .. 2^8 = 256 
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
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
