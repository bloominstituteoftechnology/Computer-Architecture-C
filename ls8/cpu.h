#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // Sprint
  int fl;
  // PC
  unsigned char pc;
  // registers (array)
  // Eight (bytes?) general-purpose registers.
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

// Define value for register.
#define SP 7

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD
};

// Memory locations.
// Where Stack Pointer (SP) is on an empty stack.
#define ADDR_EMPTY_STACK 0xF4 
// Where programs start getting loaded.
#define ADDR_PROGRAM_ENTRY 0x00 
#define CPU_FLAG 0x00

// Instructions

// Here, we are basically just implementing the instruction handlers.
// These use binary literals (codes). If these aren't available with your compiler, hex
// literals should be used.
#define LDI  0b10000010 // "Load immediate" instruction handler
#define PRN  0b01000111 // Print instruction handler
#define HLT  0b00000001 // Halt instruction handler
#define MUL  0b10100010  
#define PRA  0b01001000 
#define ADD  0b10100000 
#define PUSH  0b01000101 
#define POP  0b01000110 
#define CALL  0b01010000 
#define RET  0b00010001 
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations
// "extern" means "declare without defining".
extern void cpu_load(struct cpu *cpu, char *file);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
