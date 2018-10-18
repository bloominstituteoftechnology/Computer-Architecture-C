#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {

  // PC
  unsigned char pc;

  // registers (array)
  unsigned char reg[8];  // 8 registers

  // ram (array)
  unsigned char ram[256]; // 256 bytes

};


#define SP 7
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
#define MUL  0b10100010
#define PUSH 0b01000101
#define ADD  0b10100000
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
