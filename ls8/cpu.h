#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
typedef struct cpu
{
  // TODO
  // PC //stores currently executing insruction address
  unsigned char PC;
  //Flag for ALU comparison
  unsigned char FL;
  // registers (array) //registers go up to 8
  unsigned char reg[8];
  //creates stack pointer;
  unsigned int SP;
  // ram (array)  //8 bits goes up to 256
  unsigned char ram[256];
} cpu;

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_CMP

};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define MUL 0b10100010
#define POP 0b01000110 
#define RET 0b00010001
#define CMP 0b10100111
#define JMP 0b01010100
#define PUSH 0b01000101
#define CALL 0b01010000
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
