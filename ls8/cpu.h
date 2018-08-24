#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  // registers (array)
  // ram (array)
  unsigned char PC;           //program counter points to next program
  unsigned char registers[8]; // 8 registers  
  unsigned char ram[256];     // ram size
};

// ALU operations
enum alu_op {
  ALU_MUL,
  ALU_ADD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
// TODO: more instructions here. These can be used in cpu_run().
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define ADD  0b10100000
#define CALL 0b01010000
#define PUSH 0b01000101
#define POP  0b01000110
#define RET  0b00010001
#define ST   0b10000100
#define JMP  0b01010100
#define LD   0b10000011
#define PRA  0b01001000
#define IRET 0b00010011

// Function declarations

#define IM 5 // R5 is reserved as the interrupt mask (IM)
#define IS 6 // R6 is reserved as the interrupt status (IS)
#define SP 7 // R7 is reserved as the stack pointer (SP)

#define EMPTY_STACK 0xEA
#define KEY_PRESSED 0xF4

// extern void cpu_load(struct cpu *cpu);
extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif