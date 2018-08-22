#ifndef _CPU_H_
#define _CPU_H_

struct cpu {
  // Internal Registers
  unsigned char pc;
  unsigned char ir;
  unsigned char mar;
  unsigned char mdr;
  unsigned char fl;

  // Registers
  unsigned char registers[8];

  // Memory
  unsigned char ram[256];
};

// ALU Operations
enum alu_op
{
  ALU_ADD,
  ALU_SUB,
  ALU_MUL,
  ALU_DIV,
  ALU_MOD,
  ALU_INC,
  ALU_DEC,
  ALU_CMP,
  ALU_AND,
  ALU_NOT,
  ALU_OR,
  ALU_XOR,
  ALU_SHL,
  ALU_SHR
};

// Reserved Registers
#define IM 5
#define IS 6
#define SP 7

// Function declarations
extern void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB);
extern void cpu_load(struct cpu *cpu, char *program);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
