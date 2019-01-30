#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DATA_LEN 1024

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value){
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_path)
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

  int data_length = 0;

  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char line[1024];

  fp = fopen(file_path, "r");
  while(fgets(line, sizeof line, fp) != NULL){
    // printf("%s", line);
    cpu->ram[data_length] = (unsigned char) strtoul(line, NULL, 2);
    data_length++;
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

  unsigned char operandA;
  unsigned char operandB;

  unsigned char SP = 0xF4;
  unsigned char IR;
  unsigned char num_operand;
  while (running) {
    IR = cpu->ram[cpu->PC];
    num_operand = IR >> 6;
    if (num_operand == 2){
        operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
        operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
    }
    else if (num_operand == 1)
    {
        operandA = cpu_ram_read(cpu, cpu->PC+1);
    }
    else{
    }
    
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch(IR){
      //Case 1: opcode == LDI
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      //Case 2: opcode == PRN
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      //Case 3: opcode == HLT
      case HLT:
        running = 0;
        break;
      case MUL:
        cpu->registers[operandA] = cpu->registers[operandA] * cpu->registers[operandB];
        break;
      case PUSH:
        SP--;
        // printf("%x\n", SP);
        cpu->ram[SP] = cpu->registers[operandA];
        break;
      case POP:
        cpu->registers[operandA] = cpu->ram[SP];
        SP++;
        // printf("%x\n", SP);
        break;

      default:
        break;
    }
    cpu->PC = cpu->PC + num_operand +1;
  }
  exit(0);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers)); 
  memset(cpu->ram, 0, sizeof(cpu->ram)); 
  cpu->registers[7] = 0xF4;
}