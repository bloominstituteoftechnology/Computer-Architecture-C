#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
 unsigned char pc;// makes sure its positve
 unsigned char reg[8];//creates an array with 8 slots in it
 unsigned char ram[256];// hold the rest of the memory
};
// specail register Values
#define SP 5
//ALU operations
enum alu_op{
    ALU_MUL,
    ALU_ADD
};
//Memory locations
#define ADDR_PROGRAM_ENTRY 0x00//where program start getting loaded
#define ADDR_EMPTY_STACK 0xF4//where SP is in an empty stack

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
#define ADD  0b10100000
#define CALL 0b01010000
#define HLT  0b00000001
#define LDI  0b10000010
#define MUL  0b10100010
#define POP  0b01000110
#define PRN  0b10000111
#define PUSH 0b01000101
#define RET  0b00010001
// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
