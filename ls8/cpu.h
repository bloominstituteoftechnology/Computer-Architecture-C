#ifndef _CPU_H_
#define _CPU_H_
//init pull

// Holds all information about the CPU
struct cpu
{
  // TODO
  unsigned char SP;
  unsigned int PC;
  unsigned char reg[8];
  unsigned char ram[256];
};

// ALU operations
enum alu_op
{
  ALU_MUL,
  ALU_ADD,
  ALU_INC,
  ALU_DEC
  // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define PUSH 0b01000101
#define POP 0b01000110
#define HLT 0b00000001
#define INC 0b01100101
#define DEC 0b01100110
#define PRN 0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address);
extern void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address);

#endif
