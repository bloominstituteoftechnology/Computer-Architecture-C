#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR){
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR){
  cpu->ram[MAR] = MDR;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *fp = fopen(filename, "r");
  char line[256];

  if (fp == NULL){
    printf("Cannot open %s. Invalid file name or path.\n", fp);
    exit(2);
  }

  while(fgets(line, sizeof(line), fp) != NULL){
    if (line[0] == "#" || line[0] == "\n"){
      continue;
    }
    unsigned char command;
    command = strtoul(line, NULL, 2);
    cpu_ram_write(cpu, address, command);
    // printf("%d\n", cpu->ram[address]);
    address++;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char operandA, unsigned char operandB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[operandA]*=cpu->reg[operandB];
      break;

    // TODO: implement more ALU ops
  }
}

void call_handler(struct cpu *cpu, unsigned char operandA){
    cpu_ram_write(cpu, --cpu->reg[7], cpu->PC+2);
    cpu->PC = cpu->reg[operandA];
}

void pop_handler(struct cpu *cpu, unsigned char operandA){
  cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[7]);
  cpu->reg[7]++;
}

void push_handler(struct cpu *cpu, unsigned char operandA){
  cpu->reg[7]--;
  cpu_ram_write(cpu, cpu->reg[7], cpu->reg[operandA]);
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
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
    unsigned char move_pc = (IR >> 6) + 1;

    // printf("cpu->PC %d      IR %d\n", cpu->PC, IR);
    // printf("operandA %d     reg[operandA] %d\n", operandA, cpu->reg[operandA]);
    // printf("operandB %d     reg[operandB] %d\n", operandB, cpu->reg[operandB]);
    // printf("move_pc %d\n", move_pc);

    switch(IR) {
      case CALL:
        call_handler(cpu, operandA);
        break;
      case LDI:
        cpu->reg[operandA] = operandB;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case POP:
        pop_handler(cpu, operandA);
        printf("%d\n", cpu->reg[7]);
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;
      case PUSH:
        push_handler(cpu, operandA);
        break;
      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->reg[7]++);
        break;
      case HLT:
        running = 0;
        break;
    }
    cpu->PC += move_pc;
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
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

