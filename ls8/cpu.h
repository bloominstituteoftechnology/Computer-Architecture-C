#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC; // the type for a single unsigned byte in C; register the pc
  // registers (array)
  unsigned char reg[8]; // create an array with 8 slots in it; we have to specify unsigned with a char otherwise the compiler will be the one to use which one to use
  // ram (array)
  unsigned char ram[256]; // 256 bytes for the RAM; holds the rest of the memory
};

// ALU operations // Math operations 
enum alu_op { // enum stands for enumerated type
  ALU_MUL,  // MUL stands for multiply; assigned an integer value and defaults to starting at 0
  ALU_ADD
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010  // load
#define PRN 0b01000111  // print; 
#define HLT 0b00000001  // halt; start the program
#define MUL 0b10100010  // multiply

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

// extern void cpu_load(struct cpu *cpu);

extern void cpu_load(struct cpu *cpu, char *filename);  // predefining and instructing the compiler to expect these functions
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);  // declare function for reading from ram
extern void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value); // declare function for writing to ram

#endif
