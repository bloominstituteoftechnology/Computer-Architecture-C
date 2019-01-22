#include "cpu.h"
#include "string.h"
#include "stdio.h"

#define DATA_LEN 6

cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char *value)
{
  cpu->ram[mar] = value;
  return 0;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  char data[DATA_LEN] = {
    argv[1]
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
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

  unsigned int IR; //variable that will storevalue of current instruction

  while (running)
  {
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);               //gets PC value
                                                   // 2. Figure out how many operands this next instruction requires
                                                   // 3. Get the appropriate value(s) of the operands following this instruction
    int operandA = cpu_ram_read(cpu, cpu->PC + 1); //gets next byte past PC
    int operandB = cpu_ram_read(cpu, cpu->PC + 2); //gets bye after next past PC
    int addtopc = (IR >> 6) + 1;                   //Gives you the right number of bytes to jump to get to next instruction;
    printf("CPU Run Values: cpu->PC:%02x operandA:%02x operandB:%02x IR:%02x \n", cpu->PC, operandA, operandB, IR);
    // TODO
    // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case LDI:
      cpu->reg[operandA] = operandB; //takes value of operandA and sets it to operand b
      break;
    case PRN:
      printf("PRN: %d \n", cpu->reg[operandA]);
      break;
    case HLT:
      running = 0;
      break;
    }
        // 6. Move the PC to the next instruction.
    cpu->PC += addtopc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0; //Sets position counter to 0
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);
}
