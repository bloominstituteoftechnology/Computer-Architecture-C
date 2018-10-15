#include "cpu.h"
#include <stdio.h>

#define DATA_LEN 6


unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;
}

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
    case LOAD_VALUE:
      cpu->registers[(int)regA] = regB;
      break;
    case PRINT_VALUE:
      printf("\nValue at register %d is: %d\n", (int)regA, cpu->registers[(int)regA]);
      break;
    case HALT:
      break;
    default:
      printf("Default case called in alu\n");
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

    // 1. Get the value of the current instruction (in address PC).
    int pc = cpu->pc;
    unsigned int binary_instruction = cpu_ram_read(cpu, pc);
    unsigned int operandA = cpu_ram_read(cpu, pc + 1);
    unsigned int operandB = cpu_ram_read(cpu, pc + 2);
    enum alu_op instruction;


    // 2. switch() over it to decide on a course of action.
    switch (binary_instruction)
    {
      case LDI:
        instruction = LOAD_VALUE;
        break;
      case PRN:
        instruction = PRINT_VALUE;
        break;
      case HLT:
        instruction = HALT;
        break;
      default:
        instruction = NOTHING;
        break;
    }


    // 3. Do whatever the instruction should do according to the spec.
    alu(cpu, instruction, operandA, operandB);


    // 4. Move the PC to the next instruction. Add 1 to account for instruction argument
    int index_increment = (int) (binary_instruction >> 6) + 1;

    cpu->pc = cpu->pc + index_increment;


    if(instruction == HALT){
      running = 0;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  // TODO: Zero registers and RAM
  
  for(int i = 0; cpu->registers[i] != '\0'; i++)
  {
    cpu->registers[i] = 0;
  }
  
  for(int i = 0;  cpu->ram[i] != '\0'; i++)
  {
    cpu->ram[i] = 0;
  }
  
}
