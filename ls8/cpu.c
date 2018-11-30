#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Add RAM functions read and write
 */
 unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR) 
 {
   return cpu->ram[MAR];
 }

 void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
 {
   cpu->ram[MAR] = MDR;
 }

 void cpu_jmp(struct cpu *cpu, unsigned char DST)
 {
   cpu->PC = DST;
 }

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{

  int address = 0;

  // TODO: Replace this with something less hard-coded
  FILE *fp = fopen(filename, "r");
  char line[8192];

  if (fp == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  while (fgets(line, sizeof line, fp) != NULL) {
    // char *endchar;
    // if (line[0] == 0 || line[0] == 1) {}
    cpu->ram[address++] = strtoul(line, NULL, 2);
    if (line == NULL) {
      continue;
    }
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] = cpu->reg[regB] * cpu->reg[regA];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regB] + cpu->reg[regA];
      break;

    case ALU_CMP:
      // printf("regA is %d, regB is %d at start of alu_CMP\n", regA, regB);
      if (cpu->reg[regA] < cpu->reg[regB]) {
        cpu->FL = 0b00000100;
      }
      if (cpu->reg[regA] == cpu->reg[regB]) {
        cpu->FL = 0b00000001;
      }
      if (cpu->reg[regA] > cpu->reg[regB]) {
        cpu->FL = 0b00000010;
      }
      break;

    default:
      printf("ALU default case reached\n");
      break;
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
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);

    unsigned char advance_pc = (IR >> 6) + 1;
    // 2. switch() over it to decide on a course of action.
    switch(IR) {

      case HLT:
        running = 0;
        break;

      case LDI:
        cpu->reg[operandA] = operandB;
        // printf("LDI case triggered, PC is %d\n", cpu->PC);
        cpu->PC += advance_pc;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC += advance_pc;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += advance_pc;
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC += advance_pc;
        break;

      case PUSH:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
        cpu->PC += advance_pc;
        break;

      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]);
        cpu->reg[SP]++;
        cpu->PC += advance_pc;
        break;

      case CALL:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->PC + advance_pc);
        cpu->PC = cpu->reg[operandA];
        break;

      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->reg[SP]);
        cpu->reg[SP]++;
        break;

      case CMP:
        // printf("FL is %d at start of CMP\n", cpu->FL);
        alu(cpu, ALU_CMP, operandA, operandB);
        // printf("CMP ran, r0 is %d, r1 is %d\n", cpu->reg[operandA], cpu->reg[operandB]);
        cpu->PC += advance_pc;
        // printf("FL is %d at end of CMP\n", cpu->FL);
        break;

      case JMP:
        cpu_jmp(cpu, cpu->reg[operandA]);
        break;

      case JEQ:
      // printf("FL is %d at start of JEQ\n", cpu->FL);
        if (cpu->FL == 1) {
          cpu_jmp(cpu, cpu->reg[operandA]);
          // printf("JEQ true, PC is %d\n", cpu->PC);
          break;
        }
        // printf("JEQ false PC is %d\n", cpu->PC);
        cpu->PC += advance_pc;
        break;

      case JNE:
        if (cpu->FL != 1) {
          // printf("cpu->pc is %d\n", cpu->PC);
          // printf("DST is %d\n", cpu->reg[operandA]);
          cpu_jmp(cpu, cpu->reg[operandA]);
          // printf("JNE true\n");
          // printf("cpu->pc is %d\n", cpu->PC);
          break;
        }
        // printf("JNE false PC is %d\n", cpu->PC);
        cpu->PC += advance_pc;
        break;

      case NOP:
      // printf("NOP case triggered\n");
      cpu->PC += advance_pc;
        break;

      default:
        printf("Default case triggered\n");
        cpu->PC += advance_pc;
        break;

    }
    // if (cpu->FL <= 4) {
    //   cpu->PC += advance_pc;
    //   printf("the FL is %d\n", cpu->FL);
    //   printf("register 2 is %d\n", cpu->reg[2]);
    // }
    // else {
    //   cpu->FL = 0b00000000;
    //   // printf("FL is now %d\n", cpu->FL);
    //   continue;
    // }
    
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
  cpu->FL = 0;

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);
  memset(cpu->ram, 0, sizeof cpu->ram);
  cpu->reg[SP] = 0xf4;
}
