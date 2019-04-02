#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  // TODO
  // PC
  unsigned int PC;
  // registers (array)
  unsigned char registers[10];
  // ram (array)
  unsigned char ram[10];
};

// ALU operations
enum alu_op
{
  ALU_MUL, // what is this???
  ALU_ADD  // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
// TODO: more instructions here. These can be used in cpu_run().
#define MUL 0b10100010
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define ADD 0b10100000

// Function declarations

extern void cpu_load(struct cpu *cpu){

}; // what is this??
extern void cpu_init(struct cpu *cpu)
{
  // The `cpu_init()` function takes a pointer to
  // a `struct cpu` and initializes it as necessary.At first,
  // the PC, registers, and RAM should be cleared
  // to zero.(`memset()` might help you clear
  // registers and RAM.)

  // R0 - R6 are cleared to 0
  for (int i = 0; i < 6; i++)
  {
    // cpu->PC = '0';
    cpu->registers[i] = 0;
  }
  // R7 is set to 0xF4
  cpu->registers[7] = '0xF4';
  // PC is set to 0
  cpu->PC = 0;
  // RAM is set to 0
  void memset(cpu->ram, 0, sizeof(cpu->ram));
};
extern void cpu_run(struct cpu *cpu){

};

#endif
