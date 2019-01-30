#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
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
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    unsigned char operand1;
    unsigned char operand2;  
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    if(num_operands == 2){
      operand1 = cpu_ram_read(cpu, cpu->PC+1);
      operand2 = cpu_ram_read(cpu, cpu->PC+2);
    }
    else if(num_operands == 1){
      operand1 = cpu_ram_read(cpu, cpu->PC+1);
    }else{}
    // 4. switch() over it to decide on a course of action.
    switch(instruction){
    // 5. Do whatever the instruction should do according to the spec.
      case HLT:
      running = 0;
      break;

      case LDI:
      cpu->reg[operand1] = operand2;
      break;

      case PRN:
      fprint("%d\n", cpu->reg[operand1]);
      break;

      default:
      printf("Unknown Instruction\n");
      exit(1);
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu){
  cpu->PC = 0x00; 
  cpu->reg[SP] = 0xF4; 
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  // TODO: Initialize the PC and other special registers
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address){
  cpu->ram[address] = value;
}