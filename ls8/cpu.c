#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *arg)
{
  if (argc < 2){
    printf("please provide additional argument\n");
  } else {
    FILE *fp;
    fp = fopen(arg, "r");
    if (fp == NULL){
      printf("could not locate file");
    }
    char instruction[9];
    for (int i=0; i<256; i++){
      if (fgets(instruction, 9, fp) != NULL) {
        cpu->ram[i] = strtoul(instruction, NULL, 2);
        while(fgetc(fp) != '\n');
      } else {
        break;
      }
    }
    fclose(fp);
  }
}

/**
 * ALU
 */
int alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      return regA * regB;
      break;
    case ALU_ADD:
      return regA + regB;
      break;

    // TODO: implement more ALU ops
  }
}

char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, int item)
{
  cpu->ram[index] = item;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    int c = cpu->PC;
    unsigned char instruction = cpu->ram[c];
    unsigned char regA = cpu->registers[(cpu->ram[c+1])];
    unsigned char regB = cpu->registers[(cpu->ram[c+2])];
    unsigned char SP = cpu->registers[7];
    unsigned char SP_Val = cpu->ram[SP];
    switch(instruction) {
      case LDI:
        cpu->registers[(cpu->ram[c+1])] = cpu->ram[c+2];
        cpu->PC += 3;
        break;
      case CALL:
        //push next instruction onto stack
        --cpu->registers[7];
        cpu->ram[cpu->registers[7]] = c+2;
        //change PC to value at R[1]
        cpu->PC = cpu->registers[1];
        printf("call\n");
        break;
      case RET:
        //pop insruction pointer from stack and update PC
        cpu->PC = cpu->ram[cpu->registers[7]];
        ++cpu->registers[7];
        printf("return to instruction: %d\n", cpu->PC);
        break;
      case PUSH:
      printf("push\n");
        --cpu->registers[7];
        cpu->ram[cpu->registers[7]] = cpu->registers[(cpu->ram[c+1])];
        cpu->PC += 2;
        break;
      case POP:
      printf("pop\n");
        cpu->registers[(cpu->ram[c+1])] = cpu->ram[cpu->registers[7]];
        ++cpu->registers[7];
        cpu->PC += 2;
        break;
      case JMP:
        cpu->PC = regA;
        break;
      case CMP:
        if (regA > regB) {
          cpu->FL[5] = 0;
          cpu->FL[6] = 1;
          cpu->FL[7] = 0;
          cpu->PC += 3;
        } else if (regA == regB) {
          cpu->FL[5] = 0;
          cpu->FL[6] = 0;
          cpu->FL[7] = 1;
          cpu->PC += 3;
        } else if (regA < regB) {
          cpu->FL[5] = 1;
          cpu->FL[6] = 0;
          cpu->FL[7] = 0;
          cpu->PC += 3;
        }
        break;
      case JEQ:
        if (cpu->FL[7] == 1){
          cpu->PC = regA;
        } else {
          cpu->PC += 2;
        };
        break;
      case JNE:
        if (cpu->FL[7] == 0){
          cpu->PC = regA;
        } else {
          cpu->PC += 2;
        };
        break;
      case ALU_MUL:
        printf("mul: %d * %d\n", regA, regB);
        cpu->registers[(cpu->ram[c+1])] = alu(cpu, ALU_MUL, regA, regB);
        cpu->PC += 3;
        break;
      case ALU_ADD:
        printf("add: %d + %d\n", regA, regB);
        cpu->registers[(cpu->ram[c+1])] = alu(cpu, ALU_ADD, regA, regB);
        cpu->PC += 3;
        break;
      case PRN:
        printf("print: %d\n", cpu->registers[(cpu->ram[c+1])]);
        cpu->PC += 2;
        break;
      case HLT:
        running = 0;
        printf("fin\n");
        break;
      default:
        printf("could not read instruction\n");
        running = 0;
        break;
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
  cpu->registers[7] = 0xF4;
  // TODO: Zero registers and RAM
  memset(&cpu->registers[0], 0, 8);
  memset(&cpu->FL[0], 0, 8);
  memset(&cpu->ram[0], 0, 256);
}
