#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
typedef struct cpu
{
  // TODO
  // PC (program counter) also called IP (instruction pointer)
  unsigned char pc;

  // registers (array)
  char registers[8];

  // ram (array)
  char ram[256];
} cpu;

// ALU operations
enum alu_op
{
  ALU_MUL,
  CMP
  // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define MUL 0b10100010
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define CMP 0b10100111

//Stack set to register 7 (cpu->registers[7])
//Flags set to register 6 (cpu->registers[6])

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *file);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
// extern char *cpu_read_ram(struct cpu *cpu, unsigned char index);
// extern void cpu_write_ram(struct cpu *cpu, unsigned char index, unsigned char *value);

#endif
