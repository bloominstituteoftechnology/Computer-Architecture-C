#include "cpu.h"
#include <stdio.h>
#include <string.h>
#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}
 
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp; 
  char line[1024];
  int address = 0;

  fp = fopen(file, "r");
  while(fgets(line, sizeof(line), fp) != NULL){
    char *endptr; 
    unsigned char value;
    value = strtoul(line, &endptr, 2);

    if(value == HLT) {
      cpu->ram[address++] = value;
      break;
    }
    cpu->ram[address++] = value;
  }
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
    unsigned char ir = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 4. switch() over it to decide on a course of action.
    switch (ir)
    {
      case LDI:
      {
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        break;
      }
      case PRN:
      {
        printf("%d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;
      }
      case HLT:
      {
        running = 0;
        break;
      }
      default:
      {
        break;
      }
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->ram, 0, 8 * sizeof(unsigned char));
  memset(cpu->registers, 0, 256 * sizeof(unsigned char));
}
