#include "cpu.h"
#include <stdio.h> 
#include <string.h> 

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

int cpu_ram_read(struct cpu *cpu, int position){
  return cpu->ram[position]; // they didn't really specify what they wanted this to do so hopefully this works
}

int cpu_ram_write(struct cpu *cpu, unsigned int position, unsigned char value){
  cpu->ram[position] = value;
  return cpu->ram[position];
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
    unsigned int IR = cpu_ram_read(cpu, cpu->pc); //Instruction Register
    unsigned int num_of_operations = IR>>6; // bitwise shift operator I found / get the last two numbers 
    int operandA = cpu_ram_read(cpu, cpu->pc + 1); //next value
    int operandB = cpu_ram_read(cpu, cpu->pc + 2); //next next value
    // Use Switch statement

    //what is this "opcode they speak of lol what am i looking for"
  //   the machine code value of the instruction (e.g. `10000010` for `LDI`), also
  // known as the _opcode_

  //LS8-spec.md -> Instruction set -> Glossary
    switch (IR)
    {
      case LDI:/* Set the value of a register to an integer. */
        cpu->registers[operandA] = operandB;
        break;
      // case NOP://does nothing
      //   break;
      case HLT: //exits the program
        running = 0;
        break;
      case PRN:
        printf("PRN Says: %d \n", cpu->registers[operandA]);
        break;
    
      default:
        break;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
      //Skipping everything that says it is run in alu
    // 6. Move the PC to the next instruction.
    cpu->pc += num_of_operations + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers, use mem set
  // https://www.geeksforgeeks.org/memset-c-example/
  // At first, the PC, registers, and RAM should be cleared to zero.
  cpu->pc = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

