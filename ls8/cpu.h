#ifndef _CPU_H_
#define _CPU_H_

#define MAX_ADDR 0xff
#define CPU_FLAG 0x00

// The CPU structure which holds all information about the CPU.
// Basically, it's a collection of variables of different types.
typedef struct cpu {
  // TODO
  int fl;
  // PC
  unsigned char pc;
  // registers (array)
  // Eight (bytes?) general-purpose registers.
  unsigned char reg[8];
  // ram (array)
  unsigned ram[MAX_ADDR + 1];
} CPU;

// Special values for register.
#define SP 7
#define IS 6
#define IM 5

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD
};

// Memory locations.
// Where Stack Pointer (SP) is on an empty stack.
#define START_OF_STACK_ADDR 0xf4 
// Where programs start getting loaded.
#define START_OF_PROGRAM_ADDR 0x00 
// Others
#define INTERRUPT_MASK 0X00
#define INTERRUPTS 0X00

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
#define INT  0b01010010 
#define CMP  0b10100111
#define JMP  0b01010100
#define JEQ  0b01010101
#define JNE  0b01010110

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations
// "extern" means "declare without defining".
extern void cpu_load(CPU *cpu, char *file);
extern void cpu_init(CPU *cpu);
extern void cpu_run(CPU *cpu);

extern unsigned char cpu_ram_read(CPU *cpu, unsigned char index);
extern void cpu_ram_write(CPU *cpu, unsigned char MAR, unsigned char MDR);

#endif
