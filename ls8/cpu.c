#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "cpu_instr.h"

/**
 * Performs operations for ALU -> TODO: implement more ALU ops
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param op {enum alu_op} ALU operation enumerator value.
 * @param regA {unsigned char} Value stored in register A.
 * @param regB {unsigned char} Value stored in register B.
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char a = cpu->registers[regA];
  unsigned char b = cpu->registers[regB];

  switch (op)
  {
    case ALU_ADD:
      cpu->registers[regA] = a + b;
      break;
    case ALU_AND:
      cpu->registers[regA] = a & b;
      break;
    case ALU_CMP:
      if (a == b)
        cpu->fl = E;
      if (a < b)
        cpu->fl = L;
      if (a < b)
        cpu->fl = G;
      break;
    case ALU_DEC:
      cpu->registers[regA] = a--;
      break;
    case ALU_DIV:
      cpu->registers[regA] = a / b;
      break;
    case ALU_INC:
      cpu->registers[regA] = a++;
      break;
    case ALU_MOD:
      cpu->registers[regA] = a % b;
      break;
    case ALU_MUL:
      cpu->registers[regA] = a * b;
      break;
    case ALU_NOT:
      cpu->registers[regA] = ~a;
      break;
    case ALU_OR:
      cpu->registers[regA] = a | b;
      break;
    case ALU_SHL:
      cpu->registers[regA] = a << b;
      break;
    case ALU_SHR:
      cpu->registers[regA] = a >> b;
      break;
    case ALU_SUB:
      cpu->registers[regA] = a - b;
      break;
    case ALU_XOR:
      cpu->registers[regA] = a ^ b;
      break;
  }
}

/**
 * Reads from RAM address in MAR and stores value in MDR
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_ram_read(struct cpu *cpu)
{
  cpu->mdr = cpu->ram[cpu->mar];
}

/**
 * Writes value in MDR to RAM address in MAR
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_ram_write(struct cpu *cpu)
{
  cpu->ram[cpu->mar] = cpu->mdr;
}

/**
 * Sets instruction register to the next instruction given by PC
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void get_next_instruction(struct cpu *cpu)
{
  cpu->mar = cpu->pc;
  cpu_ram_read(cpu);
  cpu->ir = cpu->mdr;
}

/**
 * Checks if current instruction is a PC mutator
 * PC mutators will set the PC, removing the need to increment PC
 * 
 * @param instr {unsigned char} Current instruction.
 */
int isPCMutator(unsigned char instr)
{
  instr <<= 3;
  return (instr >>= 7) == 1;
}

/**
 * Sets given operand to value at memory address
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void set_operand(struct cpu *cpu, unsigned char address, unsigned char *op)
{
  cpu->mar = address;
  cpu_ram_read(cpu);
  *op = cpu->mdr;
}

/**
 * Gets number of operands in instruction, sets them to variables and updates PC
 * 
 * @param opA {unsigned char*} Pointer to operandA variable.
 * @param opB {unsigned char*} Pointer to operandB variable.
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void set_operands(unsigned char *opA, unsigned char *opB, struct cpu *cpu)
{
  unsigned char ops = cpu->ir >> 6;
  *opA = '\0';
  *opB = '\0';

  if (ops > 0)
    set_operand(cpu, ++cpu->pc, opA);

  if (ops > 1)
    set_operand(cpu, ++cpu->pc, opB);
}

/**
 * Initialize a CPU struct
 * 
 * Initializes the PC and other special registers. -> TODO
 * Zeros registers and RAM. -> TODO
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0b00000000;
  cpu->ir = 0b00000000;
  cpu->fl = 0b00000000;
  cpu->registers[SP] = 0xF4;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param program {char*} Pointer to program path.
 */
void cpu_load(struct cpu *cpu, char *program)
{
  FILE *fp = fopen(program, "r");
  char *line = NULL;
  size_t len = 9;
  int data_len = 0;
  int address = 0;
  unsigned char data[256];

  if (fp == NULL)
  {
    printf("error: could not open program %s", program);
    exit(1);
  }

  while (getline(&line, &len, fp) != -1)
  {
    if (line[0] != '#')
      data[data_len++] = strtoul(line, NULL, 2);
  }

  for (int i = 0; i < data_len; i++)
    cpu->ram[address++] = data[i];

  free(line);
  fclose(fp);
}

/**
 * Run the CPU
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_run(struct cpu *cpu)
{
  int pcMutator = 0;
  unsigned char opA = '\0';
  unsigned char opB = '\0';
  handler *branch_table = malloc(255 * sizeof *branch_table);
  load_cpu_instructions(branch_table);

  while (1) {
    get_next_instruction(cpu);

    if (cpu->ir == HLT)
      break;

    pcMutator = isPCMutator(cpu->ir);
    set_operands(&opA, &opB, cpu);
    branch_table[cpu->ir](cpu, opA, opB);

    if (!pcMutator)
      cpu->pc++;
  }

  free(branch_table);
}