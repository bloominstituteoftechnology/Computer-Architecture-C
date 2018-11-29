#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, int PC)
{
  //for(int y = 0; y < sizeof(cpu->ram)/sizeof(cpu->ram[0]); y++){
  unsigned char target = cpu->ram[PC];
/*     for(int i = 0; i < strlen(cpu->registers); i++) {

    } */
  return target;
}

unsigned char cpu_ram_write(struct cpu *cpu)
{
  cpu_load(cpu);
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char *reg = cpu->registers;
  unsigned char *ram = cpu->ram;
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    unsigned char IR = cpu->PC;
    switch(IR) {
      case LDI :
        reg[0] = 8;
        IR++;
      case PRN :
      printf('%d', reg[0]);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);
}
