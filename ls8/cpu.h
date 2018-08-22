#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char pc;
  // registers (array)
  // Eight (bytes?) general-purpose registers.
  unsigned char reg[8];
  // ram (array)
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD
};

// Instructions

// Here, we are basically just implementing the instruction handlers.
// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.
#define LDI  0b10000010 // "Load immediate" instruction handler
#define PRN  0b01000111 // Print instruction handler
#define HLT  0b00000001 // Halt instruction handler
#define MUL  0b10100010
#define ADD  0b10100000

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations
// "extern" means "declare without defining".
extern void cpu_load(struct cpu *cpu, char *file);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
