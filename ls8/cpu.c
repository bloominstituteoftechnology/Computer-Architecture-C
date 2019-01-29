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
    unsigned char IR = cpu->registers[cpu->PC];
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.

    // unsigned char num_operand = IR >> 6;
    // switch(num_operand){
    //   //Case 1: need 2 operands
    //   case 0b10: 
    //   //Case 2: need 1 operand
    //   case 0b01:
    // }
    switch(IR){
      //Case 1: opcode == LDI
      case 0x82:
        unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
        cpu->registers[operandA] = operandB;
        cpu->PC = cpu->PC+3;
      //Case 2: opcode == PRN
      case 0x47:
        unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
        printf("%d\n", operandA);
        cpu->PC = cpu->PC +2;
      //Case 3: opcode == HLT
      case 0x00:
        running = 0;
        exit();
    }
  }
  exit();
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->registers, 0, 1*sizeof(unsigned char)); 
  memset(cpu->ram, 0, 1*sizeof(unsigned char)); 
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value){
  cpu->ram[index] = value;
}