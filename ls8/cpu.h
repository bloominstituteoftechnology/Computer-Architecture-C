#ifndef _CPU_H_
#define _CPU_H_

// TODO: line 19  cpu.h -- more instructions

// ## Step 1: Implement `struct cpu` in `cpu.h`

// This structure holds information about the CPU and associated components.

// The type for a single unsigned byte in C is:

// ```c
// unsigned char x;
// ```

// (Weirdly in C, if you don't specific `signed` or `unsigned` with a `char`, it's
// up to the compiler which it uses.

// Holds all information about the CPU


struct cpu {
  // TODO
  // PC  PC = program counter...address of current instruction
  // IR = instruction register
  // MAR = memory address
  // MDR = memory data register
  // FL = flag

    unsigned char PC;
    unsigned char IR;
    unsigned char MAR;
    unsigned char MDR; 
    unsigned char FL;

    unsigned char reg[8];
    unsigned char ram[256];


  // registers (array)
  // ram (array)
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010; 
//not sure how we came upon the '0b' ... I know that it means 00000bbb and that it's a register number....but is it arbitrary? is it planned?
#define ADD 10100000;
#define AND 10101000;
#define CALL 01010000;
#define CMP 10100111;
#define DEC 01100110;
#define DIV 10100011;
#define HLT 00000001;
#define INC 01100101;
#define INT 01010010;
#define IRET 00010011;
#define JEQ 01010101;
#define JGE 01011010;
#define JGT 01010111;
#define JLE 01011001;
#define JLT 01011000;
#define JMP 01010100;
#define JNE 01010110;
#define LD 10000011;
#define MOD 10100100;
#define MUL 10100010;
#define NOP 00000000;
#define NOT 01101001;
#define OR 10101010;
#define POP 01000110;
#define PRA 01001000;
#define PRN 01000111;
#define PUSH 01000101;
#define RET 00010001;
#define SHL 10101100;
#define SHR 10101101;
#define ST 10000100;
#define SUB 10100001;
#define XOR 10101011;
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
