#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC --> keeps track of where we are in the program
  unsigned char PC; // --> Move me after each function is done
  unsigned char SP;
  // registers (array) --> where we push operand values
  unsigned char registers[8]; // --> R0 -> R7
  // ram (array) --> holds everything else 
  unsigned char ram[256]; // --> 11111111

  // stack pointer & stack array  
  

};

#define ADDR_EMPTY_STACK 0xF4
#define ADDR_PROGRAM_ENTRY 0x00

// ALU operations
enum alu_op {
	ALU_MUL,  
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111

#define MUL  0b10100010
#define POP  0b01000110
#define PUSH  0b01000101
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, int argc, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
