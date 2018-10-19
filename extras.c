// cpu.c cpu_load
#if 0

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


  // Old spec
  #include "cpu.h"
#include <string.h>
#include <stdio.h>
//#include <stlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line(1024);
  unsigned char addr = 0x00;
    
  if (fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) !=NULL) {
    char *endptr;

    unsigned char b = strtoul(line, &endptr, 2);

    if endptr == line {
      //printf("Ignoring: %s", line)
      continue;
    }

    cpu_ram_write(cpu, addr++, b);
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  (void)cpu;
  (void)regA;
  (void)regB;
  unsigned char

  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR, operandA, operandB;

  while (running) {
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);

    for (int i=0; i<8; i++) {
      printf(" %02X", cpu->reg[i]);
    }

    printf("\n");

    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case PUSH:
      cpu->reg[7]--;
      cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA & 7]);
      break;

      case POP:
        cpu->reg[operandA & 7] = cpu_ram_read(cpu, cpu->reg[7]);
        cpu->reg[7]++;
        break;

      case JMP:
        cpu->PC = cpu->reg[operandA & 7];
        add_to_pc = 0;
        break;

      case CMP:
        alu(cpu, ALU_CMP, cpu->reg[operandA], cpu->reg[operandB]);
        cpu->PC += add_to_pc;
        break;
      
      case PRA:
        printf("%c\n", reg[operandA]);
        break;

      case RET:
        cpu->PC = cpu_pop(cpu);
        break;

      case CALL:
        cpu_push(cpu, cpu->PC + 2);
        cpu->PC = reg[operandA];
        break;
        
      case JNE:

      case HLT:
        running = 0;
        break;

      default:
        printf("Unknown instruction %02x\n", IR);
        exit(3);
    }
    cpu->PC += add_to_pc;
    cpu->PC &= 0xff;

    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;

  memset(cpu->ram, 0, sizeof cpu ->ram);
  memset(cpu->reg, 0, sizeof cpu ->reg);

  cpu->reg[7] = 0xF4;
}

#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  unsigned char PC;
  unsigned char reg[0];
  unsigned char ram[256];
};

// ALU operations
enum alu_op {
	ALU_MUL
  //ALU_ADD
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define MUL  0b10100010
#define JMP  0b01010100
#define PUSH 0b01000101
#define POP  0b01000110
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *filename);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif

