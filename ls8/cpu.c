#include "cpu.h"

// #define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int data[], int data_len)
{
  int address = 0;

  for (int i = 0; i < data_len; i++)
  {
    cpu->ram[address++] = data[i];
  }
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
 *  Reads from RAM
 */
char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Writes to RAM
 */
void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
}

/**
 * Perform LDI Instruction
 */
void ldi_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char operand_b = cpu_ram_read(cpu, cpu->pc + 2);
  cpu->reg[operand_a] = operand_b;
  cpu->pc += 3;
}

/**
 * Perform PRN Instruction
 */
void prn_instr(struct cpu *cpu)
{
  unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
  unsigned char print_value = cpu->reg[operand_a];
  
  printf("%u \n", print_value);

  cpu->pc += 2;
}

/**
 * Perform MUL Instruction
 */
void mul_instr(struct cpu *cpu)
{
   unsigned char operand_a = cpu_ram_read(cpu, cpu->pc + 1);
   unsigned char operand_b = cpu_ram_read(cpu, cpu->pc + 2);
   cpu->reg[operand_a] = cpu->reg[operand_a] * cpu->reg[operand_b];
   cpu->pc += 3;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    unsigned char c_instr = cpu_ram_read(cpu, cpu->pc);
    switch (c_instr)
    {
    case LDI:
      ldi_instr(cpu);
      break;
    case PRN:
      prn_instr(cpu);
      break;
    case MUL:
      mul_instr(cpu);
      break;
    case HLT:
      running = 0;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0;
  memset(cpu->ram, 0, 256 * sizeof(unsigned char));
  memset(cpu->reg, 0, 8 * sizeof(unsigned char));
}