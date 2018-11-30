#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char item)
{
  cpu->ram[index] = item;
}

void push(struct cpu *cpu, unsigned char targetReg) {
  cpu->registers[7]--;
  cpu->ram[cpu->registers[7]] = cpu->registers[targetReg];

}

void pop(struct cpu *cpu, unsigned char targetReg) {
  cpu->registers[targetReg] = cpu->ram[cpu->registers[7]];
  cpu->registers[7]++;
}

void call(struct cpu *cpu, unsigned char targetReg) {
  cpu->registers[7]--;
  cpu->ram[cpu->registers[7]] = cpu->PC + 2;
  cpu->PC = cpu->registers[targetReg];
}

void ret(struct cpu *cpu) {
  cpu->PC = cpu->ram[cpu->registers[7]];
  cpu->registers[7]++;
}

void cpu_load(char* filename, struct cpu *cpu)
{
  FILE *fp = fopen(filename, "r" );
  char line[1024];
  if(fp == NULL) {
    printf("Error: Could not open file.\n");
    exit(1);
  }
  int address = 0;

  while(fgets(line, sizeof line, fp) != NULL) {
    cpu->ram[address] = strtoul(line, NULL, 2);
    address++;
  }
  fclose(fp);

  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  

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

    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;

    case ALU_AND:
      cpu->registers[regA] = cpu->registers[regA] & cpu->registers[regB];
      break;
      
    case ALU_OR:
      cpu->registers[regA] = cpu->registers[regA] | cpu->registers[regB];
      break;
      
    case ALU_XOR:
      cpu->registers[regA] = cpu->registers[regA] ^ cpu->registers[regB];
      break;
      
    case ALU_NOT:
      cpu->registers[regA] = ~cpu->registers[regA];
      break;
      
    case ALU_SHL:
      cpu->registers[regA] <<= cpu->registers[regB];
      break;
      
    case ALU_SHR:
      cpu->registers[regA] >>= cpu->registers[regB];
      break;
      
    case ALU_MOD:
      cpu->registers[regA] %= cpu->registers[regB];
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

  // for loop to test broken code in while loop to prevent infiite loop and allow to see code at beginning of the loop for debugging.
  // for(int i = 0; i < 14; i++) {
  while (running) {
    unsigned char IR;
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    switch(IR)
    {
      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case HLT:
        running = 0;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC += 3;
        break;

      case PUSH:
        push(cpu, operandA);
        cpu->PC += 2;
        break;

      case POP:
        pop(cpu, operandA);
        cpu->PC += 2;
        break;

      case CALL:
        call(cpu, operandA);
        break;

      case RET:
        ret(cpu);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC += 3;
        break;

      case CMP:
        if(cpu->registers[operandA] < cpu->registers[operandB]) {
          cpu->FL = 0b00000100;
        }else if(cpu->registers[operandA] > cpu->registers[operandB]) {
          cpu->FL = 0b00000010;
        } else {
          cpu->FL = 0b00000001;
        }
        cpu->PC += 3;
        break;

      case JMP:
        cpu->PC = cpu->registers[operandA];
        break;

      case JEQ:
        if(cpu->FL == 0b00000001) {
          cpu->PC = cpu->registers[operandA];
        }else{
          cpu->PC += 2;
        }
        break;

      case JNE:
        if(cpu->FL != 0b00000001) {
          cpu->PC = cpu->registers[operandA];
        }else{
          cpu->PC += 2;
        }
        break;

      case AND:
        alu(cpu, ALU_AND, operandA, operandB);
        cpu->PC += 3;
        break;
        
      case OR:
        alu(cpu, ALU_OR, operandA, operandB);
        cpu->PC += 3;
        break;
        
      case XOR:
        alu(cpu, ALU_XOR, operandA, operandB);
        cpu->PC += 3;
        break;
        
      case NOT:
        alu(cpu, ALU_NOT, operandA, operandB);
        cpu->PC += 2;
        break;
        
      case SHL:
        alu(cpu, ALU_SHL, operandA, operandB);
        cpu->PC += 3;
        break;
        
      case SHR:
        alu(cpu, ALU_SHR, operandA, operandB);
        cpu->PC += 3;
        break;
        
      case MOD:
        if(operandB != 0) {
          alu(cpu, ALU_MOD, operandA, operandB);
          cpu->PC += 3;
        } else {
          printf("Error: Invalid Value");
          running = 0;
        }        
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
  cpu->FL = 0;
  

  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->registers[7] = 0xF4;
}
