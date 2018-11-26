#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    int ir = cpu->ram[cpu->PC];
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch (ir) {
      case -126:
        cpu->registers[cpu->ram[cpu->PC+1]] = cpu->ram[cpu->PC+2];
        cpu->PC += 2;
        break;
      case PRN:
        printf("%d\n", cpu->registers[cpu->ram[cpu->PC+1]]);
        cpu->PC++;
        break;
      case HLT:
        running = 0;
        break;
      default:
        break;
    }
    // 4. Move the PC to the next instruction.
    cpu->PC++;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = malloc(sizeof(struct cpu));
  cpu->PC = 0;
  memset(cpu->ram,0,256);

  for(int i = 0;i<7;i++){
    cpu->registers[i] = 0;
  }

  cpu->registers[7] = 0xF4;
}

unsigned int cpu_ram_read(struct cpu *cpu)
{
  return cpu->ram[cpu->PC++];
}

void cpu_ram_write(struct cpu *cpu, int item)
{
  cpu->ram[cpu->PC] = item;
  cpu->PC++;
}
