#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  int PC;
  int registers[8];
  int ram[256];
	unsigned char FL;
};

// ALU operations
enum alu_op {
	ALU_MUL,
	ALU_ADD
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
#define CALL 0b01010000
#define RET  0b00010001
#define ADD  0b10100000
#define CMP  0b10100111
#define JEQ  0b01010101
#define JGE  0b01011010
#define JGT  0b01010111
#define JLE  0b01011001
#define JLT  0b01011000
#define JMP  0b01010100
#define JNE  0b01010110
#define LD   0b10000011
#define PRA  0b01001000
#define INC  0b01100101
#define DEC  0b01100110
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file_name);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char loc);
extern void cpu_ram_write(struct cpu *cpu, unsigned char loc, int item);

#endif
