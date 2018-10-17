#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}
// val = value
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val)
{
  cpu->ram[address] = val;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *file = fopen(filename, "r");
  char line[1024];
  int address = 0;
// reads all the lines, ignores ones that were not read, stores in ram
  while(fgets(line,sizeof line,file) !=NULL){
    if (line[0]=='\n' || line[0] =='#'){
      continue;
    }
    
    cpu_ram_write(cpu, address++, strtol(line, NULL, 2));
  }
  fclose(file);
  //line below (26-40)commented out and replaced with a "less hard coded" solution
  /* char data[DATA_LEN] = {
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
  } */

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
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
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
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);

    int add_to_pc = (IR >> 6) + 1;

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    /* for (int i = 0; i < 8; i++)
    {
      printf(" %02X", cpu->reg[i]);
    } */

    // POP stack
    void handle_POP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
    {
      cpu->reg[operandA] = cpu->ram[cpu->reg[SP]];

      cpu->reg[SP]++;

      
    }

    unsigned handle_PUSH(struct cpu * cpu, unsigned char operandA, unsigned char operandB)
    {
      cpu->reg[SP]--;

      cpu->ram[cpu->reg[SP]] = cpu->reg[operandA];
    }

    // 2. switch() over it to decide on a course of action.
    switch (IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case HLT:
        running = 0;
        break;

      default:
        fprintf(stderr, "ERROR %02X: finding instruction %02X\n", add_to_pc, IR);
        exit(1);
      }
      // 3. Do whatever the instruction should do according to the spec.
      if (!add_to_pc){

      }
      // 4. Move the PC to the next instruction.
      cpu->PC = add_to_pc;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->reg, 0, sizeof cpu->reg);

  cpu->reg[7] = 0b11110100;
}
