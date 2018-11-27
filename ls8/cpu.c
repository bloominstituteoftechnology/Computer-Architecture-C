#include "cpu.h"
#include "stdio.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    // need to parse an external file
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
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  printf("\nCPU running\n");

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    unsigned char operandA = cpu->ram[(cpu->PC + 1) & 0xFF];
    unsigned char operandB = cpu->ram[(cpu->PC + 2) & 0xFF];

    for (int i = 0; i<8; i++) {
      printf("%d\n", cpu->registers[i]);
    }

    // 2. switch() over it to decide on a course of action.
    switch(IR) {
          // 3. Do whatever the instruction should do according to the spec.
      case LDI:
      printf(" ....... LDI ...... \n");
      cpu->registers[operandA] = operandB;
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      break;

      case HLT:
      printf(" ....... HLT ...... \n");
      printf("RAM at F2: %d\n" , cpu->ram[(cpu->PC + 2) & 0xF2]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      running = 0;
      break;
    }
    // 4. Move the PC to the next instruction.
    cpu->PC += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  printf("\nCPU init\n");
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));
}
