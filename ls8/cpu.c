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
  unsigned char *reg = cpu->reg;

  // assign operands to values
  unsigned char valueA = reg[regA];
  unsigned char valueB = reg[regB];

  switch (op) {
    case ALU_MUL:
      reg[regA] *= valueB;
      break;
    case ALU_ADD:
      reg[regA] += valueB;
      break;
    case ALU_CMP:
      if (valueA == valueB) {
        cpu->FL = 1;
        // printf("even %d\n", cpu->FL);
      } else if (valueA < valueB) {
        cpu->FL = 2;
        // printf("less %d\n", cpu->FL);
      } else {
        cpu->FL = 3;
        // printf("greater %d\n", cpu->FL);
      }
      break;

    // TODO: implement more ALU ops
  }
}

void push_to_stack(struct cpu *cpu, int val) {
  cpu->reg[7]--;
  cpu->ram[cpu->reg[7]] = val;
}

unsigned char pop_from_stack(struct cpu *cpu) {
  unsigned char val = cpu->ram[cpu->reg[7]];
  cpu->reg[7]++;
  return val;
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
        exit(1);
        // return 0;
      case MUL:
        alu(cpu, ALU_MUL, operand0, operand1);
        cpu->PC += 3;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operand0, operand1);
        cpu->PC += 3;
        break;
      case PUSH:
        push_to_stack(cpu, cpu->reg[operand0]);
        cpu->PC += 2;
        break;
      case POP:
        cpu->reg[operand0] = pop_from_stack(cpu);
        cpu->PC += 2;
        break;
      case CALL:
        push_to_stack(cpu, cpu->PC + 2);
        cpu->PC = cpu->reg[operand0];
        break;
      case RET:
        cpu->PC = pop_from_stack(cpu);
        break;
      case CMP:
        alu(cpu, ALU_CMP, operand0, operand1);
        cpu->PC += 3;
        break;
      case JMP:
        cpu->PC = cpu->reg[operand0];
        break;
      case JEQ:
        if (cpu->FL == 1) {
          cpu->PC = cpu->reg[operand0];
        } else {
          cpu->PC += 2;
        }
        break;
      case JNE:
        if (cpu->FL != 1) {
          cpu->PC = cpu->reg[operand0];
        } else {
          cpu->PC += 2;
        }
        break;

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
  // set PC & flag to 0
  cpu->PC = 0;
  cpu->FL = 0;
  // set ram to 0
  memset(cpu->ram, 0, sizeof cpu->ram);

}
