#include "cpu.h"
#include <stdio.h>
#include <string.h>

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

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    // 2. switch() over it to decide on a course of action.
    switch (IR) {
            case HLT:
                int running = 0;
                exit();

            case LDI:
                unsigned char index = cpu->ram[cpu->PC + 1];
                cpu->registers[index] = cpu->ram[cpu->PC + 2];
                cpu->PC += 3;
                break;

            case PRN:
                unsigned char index = cpu->ram[cpu->PC + 1];
                printf("Saved value: %d\n", cpu->registers[index]);
                cpu->PC += 2;
                break;
        }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
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
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

// void cpu_ram_read(struct cpu *cpu)
// {
//   // access RAM inside struct cpu
//   // cpu->ram
// }

// void cpu_ram_write(struct cpu *cpu)
// {
//   // access RAM inside struct cpu
// }
