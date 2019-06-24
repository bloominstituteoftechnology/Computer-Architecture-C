#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_name) {
  FILE *fp;
  char line[128];
  int address = 0;

  fp = fopen(file_name, "r");
  if (fp == NULL) { 
    fprintf(stderr, "Error - Opening File Failed\n"); 
    exit(1); 
  }
  while (fgets(line, 128, fp) != NULL) {
    char *endptr;
    unsigned char value = strtoul(line, &endptr, 2);

    if (line == endptr) {
      continue;
    }

    cpu->ram[address++] = value;
    
  }

  fclose(fp);

}

  // TODO: Replace this with something less hard-coded


/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
  }
}
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {
  cpu->ram[index] = value;
}

void handle_PUSH(struct cpu *cpu, unsigned char value)
{
  cpu->reg[7]--;
  cpu_ram_write(cpu, cpu->reg[7], value);
}
unsigned char handle_POP(struct cpu *cpu, unsigned char value)
{
  value = cpu_ram_read(cpu, cpu->reg[7]);
  cpu->reg[7]++;
  return value;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    unsigned char operandA = 0;
	  unsigned char operandB = 0;
    // 1. Get the value of the current instruction (in address PC).
    unsigned char ir = cpu->ram[cpu->pc];
    // 2. Figure out how many operands this next instruction requires
    unsigned char num_operands = ir >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    if ( num_operands == 2) {
      operandA = cpu_ram_read(cpu, cpu->pc+1);
      operandB = cpu_ram_read(cpu, cpu->pc+2);
    } else if (num_operands == 1) {
      operandA = cpu_ram_read(cpu, cpu->pc+1);
    } else {
      return;
    }
    int cur_index = ir;
    printf("TRACE: cpu-PC: %d: cpu-IR: %02X    operand0: %02x operand1: %02x\n", cpu->pc, ir, operandA, operandB);
    // 4. switch() over it to decide on a course of action.
    switch (ir) {
    // 5. Do whatever the instruction should do according to the spec.
    case HLT:
      running = 0;
      cpu->pc += 1;
      break;
    case LDI:
      cpu->reg[operandA] = operandB;
      break;
    case PRN:
      printf("%d\n", cpu->reg[operandA]);
      break;
    case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
    case PUSH:
        handle_PUSH(cpu, cur_index);
        break;
    case POP:
        handle_POP(cpu, cur_index);
        break;
    case CALL:
        cpu->reg[7]--;
        cpu->ram[cpu->reg[7]] = cpu->pc + 2;
        cpu->pc = cpu->reg[operandA];
        num_operands = -1;
        break;
    case RET:
        cpu->pc = cpu->ram[cpu->reg[7]];
        cpu->reg[7]++;
        break;
    case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
    case CMP:
	      if (cpu->reg[operandA] == cpu->reg[operandB]) {
	        cpu->fl = 1;
	      }
	      else if (cpu->reg[operandA] < cpu->reg[operandB]) {
	        cpu->fl = 4;
	      }
	      else if (cpu->reg[operandA] > cpu->reg[operandB]) {
	        cpu->fl = 2;
	      }
	      break;
    case JMP:
        cpu->pc = cpu->reg[operandA] - num_operands - 1;
        break;
    case JEQ:
	      if (cpu->fl == 1) {
	      cpu->pc = cpu->reg[operandA] - num_operands - 1;
	      }
	      break;
	  case JNE:
	      if (cpu->fl == 2 || cpu->fl == 4) {
	      cpu->pc = cpu->reg[operandA] - num_operands - 1;
	      }
	      break;
    default:
      printf("Error Unknown Instruction");
      exit(1);
    }
    
    // 6. Move the PC to the next instruction.
     cpu->pc += (num_operands + 1);
  }
}


/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // cpu = malloc(sizeof(struct cpu));
  cpu->pc = 0;
  cpu->fl = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0 , sizeof(cpu->ram));
  cpu->reg[7] = 0xF4;

}
