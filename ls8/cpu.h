#ifndef _CPU_H_
#define _CPU_H_
// We're going to write an emulator -
// an 8-bit computer with 8-bit memory addressing
// An 8 bit CPU is one that only has 8 wires available for addresses
// (specifying where something is in memory), computations, and instructions.
//  With 8 bits, our CPU has a total of 256 bytes of memory and can only compute values up to 255.

// Holds all information about the CPU

// The type for a single unsigned byte in C is:
// unsigned char x;
//  in C, if you don't specific signed or unsigned with a char,
//  it's up to the compiler which it uses.

struct cpu
{
    // TODO
    // PC - program counter
    unsigned char pc;
    // registers (array)
    unsigned char reg[8];
    // ram (array)
    unsigned char ram[256];
};

// Other general purpose reg. names
#define SP 7

// ALU operations
enum alu_op // for debugger
{
    ALU_MUL, // 0
    ALU_ADD
    // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define MUL 0b10100010
#define HLT 0b00000001
#define PRN 0b01000111
#define PUSH 0b01000101
#define POP 0b01000110
#define JMP 0b01010100
#define CALL 0b01010000
#define RET 0b00010001

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
