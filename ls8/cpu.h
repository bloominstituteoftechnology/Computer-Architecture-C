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
typedef enum alu_op {
 ALU_MUL
}op;


struct CPU {
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

//'0b' in front of a binary number indicates binary literal...can also be '0B'. 
// '0x' in front of a hex number indicates hexadecimal literal...can also be '0X'

#define LDI  0b10000010 || 0x82; 
#define ADD 0b10100000 || 0xA0;
#define AND 0b10101000 || 0xA8;
#define CALL 0b01010000 || 0x50;
#define CMP 0b10100111 || 0xA7;
#define DEC 0b01100110 || 0x66;
#define DIV 0b10100011 || 0xA3;
#define HLT 0b00000001 || 0x01;
#define INC 0b01100101 || 0x65;
#define INT 0b01010010 || 0x52;
#define IRET 0b00010011 || 0x13;
#define JEQ 0b01010101 || 0x55;
#define JGE 0b01011010 || 0x5A;
#define JGT 0b01010111 || 0x57;
#define JLE 0b01011001 || 0x59;
#define JLT 0b01011000 || 0x58;
#define JMP 0b01010100 || 0x54;
#define JNE 0b01010110 || 0x56;
#define LD 0b10000011 || 0x83;
#define MOD 0b10100100 || 0xA4;
#define MUL 0b10100010 || 0xA2;
#define NOP 0b00000000 || 0x00;
#define NOT 0b01101001 || 0x69;
#define OR 0b10101010 || 0xAA;
#define POP 0b01000110 || 0x46;
#define PRA 0b01001000 || 0x48;
#define PRN 0b01000111 || 0x47;
#define PUSH 0b01000101 || 0x45;
#define RET 0b00010001 || 0x11;
#define SHL 0b10101100 || 0xAC;
#define SHR 0b10101101 || 0xAD;
#define ST 0b10000100 || 0x84;
#define SUB 0b10100001 || 0xA1;
#define XOR 0b10101011 || 0xAB;
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct CPU *cpu);
extern void cpu_init(struct CPU *cpu);
extern void cpu_run(struct CPU *cpu);
extern void cpu_ram_read(struct CPU *cpu);
extern void cpu_ram_write(struct CPU *cpu);

#endif
