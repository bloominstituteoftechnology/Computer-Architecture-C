#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp;
  char line[1024];
  int address = 0;

  fp = fopen(file, "r");

 	while (fgets(line, sizeof(line), fp) != NULL) {

		if (line[0] == '\n' || line[0] == '#') {
			continue;
		}

		unsigned char b;
		b = strtoul(line, NULL, 2);
    cpu->ram[address++] = b;
	}

  fclose(fp);

  // char data[DATA_LEN] = {
  //   // // From print8.ls8
  //   // 0b10000010, // LDI R0,8
  //   // 0b00000000,
  //   // 0b00001000,
  //   // 0b01000111, // PRN R0
  //   // 0b00000000,
  //   // 0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

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
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
  }
}
// *** Helper functions

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  char SP = 0XF4;
  cpu->registers[7] = SP;

  while (running) {
    // 1. Get the value of the current instruction (in address PC).
    cpu->ir = cpu_ram_read(cpu, cpu->pc);
    int argc = cpu->ir >> 6;
    unsigned char argv[2];
    for (int i = 0; i < argc; i++) {
      argv[i] = cpu_ram_read(cpu, cpu->pc + i + 1);
    }
    // 2. switch() over it to decide on a course of action.
    switch (cpu->ir) {
      // 3. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu->registers[argv[0]] = argv[1];
        break;
      case PRN:
        printf("%d\n", cpu->registers[argv[0]]);
        break;
      case HLT:
        running = 0;
        break;
      case MUL:
        alu(cpu, ALU_MUL, argv[0], argv[1]);
        break;
      case PUSH:
        cpu->registers[7] -= 1;
        cpu_ram_write(cpu, cpu->registers[7], cpu->registers[argv[0]]);
        break;
      case POP:
        cpu->registers[argv[0]] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7] += 1;
        break;
      default:
        printf("incorrect instruction");
        break;
    }
    int changed = cpu->ir >> 4 & 1;
    if (!changed) {
      cpu->pc += 1 + argc;
    }
    // 4. Move the PC to the next instruction.
    // cpu->pc += 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->ir = 0;
  cpu->fl = 0;
  // TODO: Zero registers and RAM
  for (int i = 0; i<8; i++) {
    cpu->registers[i] = 0;
  }
  for (int i = 0; i<256; i++) {
    cpu->ram[i] = 0;
  }
  // memset(cpu->registers, 0, sizeof(cpu->registers));
  // memset(cpu->ram, 0, sizeof(cpu->ram));
}