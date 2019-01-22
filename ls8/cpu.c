#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 12

/**
 * Read from RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Write to RAM
 */
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  return cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
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

  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b10000010, // LDI R1,0
    0b00000001,
    0b00000000,
    0b10100011, // DIV R0, R1
    0b00000000,
    0b00000001,
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
    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
    case ALU_AND:
      cpu->reg[regA] = cpu->reg[regA] & cpu->reg[regB];
      break;
    case ALU_CMP:
      if (cpu->reg[regA] < cpu->reg[regB])
      {
        cpu->FL = 0b00000100;
      }
      else if (cpu->reg[regA] > cpu->reg[regB])
      {
        cpu->FL = 0b00000010;
      }
      else
      {
        cpu->FL = 0b00000001;
      }
      break;
    case ALU_DEC:
      cpu->reg[regA]--;
      break;
    case ALU_DIV:
      cpu->reg[regA] /= cpu->reg[regB];
      break;
    case ALU_MUL:
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
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    switch(IR)
    {
      // 5. Do whatever the instruction should do according to the spec.
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC += (IR >> 6) + 1;
        break;
      case AND:
        alu(cpu, ALU_AND, operandA, operandB);
        cpu->PC += (IR >> 6) + 1;
        break;
      case CALL:
        cpu->reg[7]--;
        cpu_ram_write(cpu, cpu->reg[7], cpu->PC + 2);
        cpu->PC = cpu->reg[operandA];
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        cpu->PC += (IR >> 6) + 1;
        break;
      case DEC:
        alu(cpu, ALU_DEC, operandA, operandB);
        cpu->PC += (IR >> 6) + 1;
        break;
      case DIV:
        if (cpu->reg[operandB] == 0)
        {
          printf("Divide by 0 not allowed.\n");
          running = 0;
        }
        else
        {
          alu(cpu, ALU_DIV, operandA, operandB);
          cpu->PC += (IR >> 6) + 1;
        }
        break;
      case HLT:
        running = 0;
        break;
      case LDI:
        cpu->reg[operandA] = operandB;
        cpu->PC += (IR >> 6) + 1;
        break;
      case PRN:
        printf("Register: %X, Value: %d\n", operandA, cpu->reg[operandA]);
        cpu->PC += (IR >> 6) + 1;
        break;
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
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->reg, 0, 8 * sizeof(cpu->reg[0]));
  memset(cpu->ram, 0, 256 * sizeof(cpu->ram[0]));
}
