#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  unsigned char FL;
  unsigned char PC;
  unsigned char registers[8]; // registers (array)
  unsigned char ram[256];     // ram (array)
};

// define flags registers
#define FL_EQ 1
#define FL_GT 2
#define FL_LT 4

//struct to represent a stack
//The SP points at the value at the top of the stack

#define SP 7

// ALU operations
enum alu_op
{
  ALU_MUL,
  // Add more here
  ALU_ADD,
  ALU_CMP
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010 // Set the value of a register to an integer.
// TODO: more instructions here. These can be used in cpu_run().
#define HLT 0b00000001  // Halt the CPU (and exit the emulator).
#define PRN 0b01000111  // Print numeric value stored in the given register.
#define MUL 0b10100010  // Multiply the values in two registers together and store the result in registerA.
#define ADD 0b10100000  // Add the value in two registers and store the result in registerA.
#define PUSH 0b01000101 // Push the value in the given register on the stack.
#define POP 0b01000110  //Pop the value at the top of the stack into the given register.
#define CALL 0b01010000 // Calls a subroutine (function) at the address stored in the register.
#define RET 0b00010001
#define JMP 0b01010100 // Jump to the address stored in the given register.
#define CMP 0b10100111 // Compare the values in two registers.
#define JEQ 0b01010101 // If `equal` flag is set (true), jump to the address stored in the given register.
#define JNE 0b01010110 // If `E` flag is clear (false, 0), jump to the address stored in the given register.

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
