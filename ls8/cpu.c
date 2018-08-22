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
    case ALU_MUL:
      cpu->registers[regA] = a * b;
      break;
  }
}

/**
 * Reads from RAM address and stores value into store
 * 
 * @param ram {unsigned char[]} Memory array.
 * @param address {unsigned char} Address for memory array.
 * @param store {unsigned char*} Pointer to location value at memory address should be stored.
 */
void cpu_ram_read(unsigned char ram[], unsigned char address, unsigned char *store)
{
  *store = ram[address];
}

/**
 * Writes data to RAM address 
 * 
 * @param ram {unsigned char[]} Memory array.
 * @param address {unsigned char} Address for memory array.
 * @param data {unsigned char} data to be stored at memory address.
 */
void cpu_ram_write(unsigned char ram[], unsigned char address, unsigned char data)
{
  ram[address] = data;
}

/**
 * Gets number of operands in instruction, sets them to variables and updates PC
 * 
 * @param opA {unsigned char*} Pointer to operandA variable.
 * @param opB {unsigned char*} Pointer to operandB variable.
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void get_operands(unsigned char *opA, unsigned char *opB, struct cpu *cpu)
{
  unsigned char ops = cpu->ir >> 6;

  if (ops > 0)
  {
    cpu->pc++;
    cpu_ram_read(cpu->ram, cpu->pc, opA);
  }

  if (ops > 1)
  {
    cpu->pc++;
    cpu_ram_read(cpu->ram, cpu->pc, opB);
  }
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
  int running = 1;
  unsigned char operandA;
  unsigned char operandB;
  handler *branch_table = malloc(255 * sizeof *branch_table);
  load_cpu_instructions(branch_table);

  while (running) {
    cpu_ram_read(cpu->ram, cpu->pc, &cpu->ir);

    if (cpu->ir == HLT)
    {
      handle_HLT(&running);
      continue;
    }

    operandA = '\0';
    operandB = '\0';
    get_operands(&operandA, &operandB, cpu);
    branch_table[cpu->ir](cpu, operandA, operandB);
    cpu->pc++;
  }

  free(branch_table);
}