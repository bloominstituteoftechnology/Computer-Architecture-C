#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  unsigned char PC;			  // program counter
  unsigned char FL;       // flags
  unsigned char reg[8];   // 8 registers
  unsigned char ram[256]; // 256 bytes of RAM
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
// ALU ops
#define ADD  0b10100000 // 00000aaa 00000bbb
#define SUB  0b10100001 // 00000aaa 00000bbb
#define MUL  0b10100010 // 00000aaa 00000bbb
#define DIV  0b10100011 // 00000aaa 00000bbb
#define MOD  0b10100100 // 00000aaa 00000bbb

#define INC  0b01100101 // 00000rrr
#define DEC  0b01100110 // 00000rrr

#define CMP  0b10100111 // 00000aaa 00000bbb

#define AND  0b10101000 // 00000aaa 00000bbb
#define NOT  0b01101001 // 00000rrr
#define OR   0b10101010 // 00000aaa 00000bbb
#define XOR  0b10101011 // 00000aaa 00000bbb
#define SHL  0b10101100 // 00000aaa 00000bbb
#define SHR  0b10101101 // 00000aaa 00000bbb

#define PUSH 0b01000101 // 00000rrr
#define POP  0b01000110 // 00000rrr
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
