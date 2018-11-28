#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int item){
  cpu->ram[++cpu->PC] = item;
  return;
}

/**
 * MUL 
 */
int multiply(unsigned char regA, unsigned char regB){
  return regA * regB;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  
  FILE *fp;
  char line[8192];
  int address = 0;
  fp = fopen(filename, "r");
  while(fgets(line, sizeof(line), fp) != NULL){
    // printf(">>>>>>> %d\n", strtoul(line, NULL, 10));
    cpu->ram[address++] = strtoul(line, NULL, 2);
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
    unsigned char current_instruction = cpu_ram_read(cpu, cpu->PC);
    unsigned char operand1 = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operand2 = cpu_ram_read(cpu, cpu->PC+2);

    switch(current_instruction){
      case LDI:
        cpu->registers[operand1] = operand2;
        printf("Printing %d\n", cpu->registers[operand1]);
        cpu->PC = cpu->PC+3;
        break;
      case MUL:
        cpu->registers[operand1] = multiply(cpu->registers[0], cpu->registers[1]);
        printf("MUL >>> %d\n", cpu->registers[operand1]);
        cpu->PC = cpu->PC+3;
        break;
      case PRN:
        printf("Printing %d\n", cpu->registers[0]);
        cpu->PC = cpu->PC+2;
        break;
      case HLT:
        printf("LS-8 stopping.  Goodbye!\n");
        running = 0;
        break;
      default:
        exit(1);
    }
  }
  return;
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}