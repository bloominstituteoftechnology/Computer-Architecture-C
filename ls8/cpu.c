#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{
  // get the file | will get file path via argument later
  // per guided demo
  FILE *fp;
  char line[1024];

  int address = 0x00;

  if((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Unable to open file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    char *endpoint;
    unsigned char value;

    value = strtoul(line, &endpoint, 2);

    if(endpoint == line) {
      continue;
    }

    cpu->ram[address++] = value;

  }

  // TODO: Replace this with something less hard-coded
}

// MAR - memory address register | where does is go
// MDR - memory data register | what it is
void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr) {
  cpu->ram[mar] = mdr;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar) {
  return cpu->ram[mar];
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
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operand0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operand1 = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR) {
      // BE SURE CASE IS SAVED AS BINARY LITERALS IN CPU.H FILE
      case LDI:
        cpu->reg[operand0] = operand1;
        cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operand0]);
        cpu->PC += 2;
        break;
      case HLT:
        cpu->PC++;
        return 0;

      default:
        // beej printed IR & cpu->PC
        printf("unexpected command\n");
        exit(1);
    }
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // set r0-r6 to clear to 0
  for (int i = 0; i < 7; i++) {
    cpu->reg[i] = 0;
  }
  // set r7 to 0xF4
  cpu->reg[7] = 0xF4;
  // set PC to 0 | no flags yet
  cpu->PC = 0;
  // set ram to 0
  memset(cpu->ram, 0, sizeof cpu->ram);

}
