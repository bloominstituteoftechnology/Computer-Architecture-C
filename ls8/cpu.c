#include "cpu.h"
#include "string.h"
#include "stdio.h"
#include <stdlib.h>

#define DATA_LEN 6
// #define DATA_LEN 7
// #define ADDR_EMPTY_STACK 0xF4

void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
{
  cpu->ram[address] = value;
  // return 0;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_push(struct cpu *cpu, unsigned char val) 
{
cpu->SP--;
cpu_ram_write(cpu, val, cpu->SP);
}

unsigned char cpu_pop(struct cpu *cpu)
{
  return cpu_ram_read(cpu, cpu->SP++);
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  char line[1024];
  int address = 0;
  // int address = ADDR_PROGRAM_ENTRY;

  if ((fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Cannot Open File %s\n", argv[0]);
    exit(1);
  }
  while (fgets (line, sizeof line, fp) != NULL) {

    char *endchar;
    unsigned char byte = strtol(line, &endchar, 2);

    if (endchar == line) {
      continue;
    }
    // cpu_ram_write(cpu, byte, address++); 
    cpu->ram[address++] = byte;

  }
}

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;

    case ALU_INC:
      cpu->reg[regA]++;
      break;

    case ALU_DEC:
      cpu->reg[regA]--;
      break;

    case ALU_CMP:
    // cpu->FL = 0;
      if (cpu->reg[regA] == cpu->reg[regB]) {
        // set the equal bit
        cpu->FL = (cpu->FL & ~7) | 1;
      } else if (cpu->reg[regA] <= cpu->reg[regB]) {    
        // set the lest than bit
        cpu->FL = (cpu->FL & ~7) | 2;
      } else {     
        // set the greater than bit
        cpu->FL = (cpu->FL & ~7) | 4;
      }
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

    unsigned char operandA;
    unsigned char operandB;

    unsigned int instruction = cpu_ram_read(cpu, cpu->PC);
    // unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    
    // 2. Figure out how many operands this next instruction requires
    // unsigned int num_operands = (instruction >> 4) & 1;
    unsigned int num_operands = instruction >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1)& 0xff);
      operandB = cpu_ram_read(cpu, (cpu->PC + 2)& 0xff);
    } else if (num_operands == 1) {
      operandA = cpu_ram_read(cpu, (cpu->PC + 1)& 0xff);
    } else {
      
    }
    // 4. switch() over it to decide on a course of action.
    switch (instruction) {

      case HLT:
        running = 0;
        break;

      case PRN:
        // print whatever is in the specified register
        printf("%d\n", cpu->reg[operandA]);
        break;

      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;

      case PUSH:
        cpu_push(cpu, cpu->reg[operandA]);
        break;

      case POP:
        cpu->reg[operandA] = cpu_pop(cpu);
        break;

      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;

      case JMP:
        cpu->PC = operandA; 
        printf("im getting %d\n", operandA);
        // num_operands = 0;
        break;

      // case JMP:
      //   cpu->PC = cpu->reg[operandA]-2;
      //   num_operands = 0;
      //   break;

      case JNE:
          if((cpu->FL & (1<<0)) == 2) {
          cpu->PC = cpu->reg[operandA];
          }
        break;

      case JEQ:
          if((cpu->FL & (1<<0)) == 1) {
          cpu->PC = cpu->reg[operandA];
          }
        break; 
        
      // default:
      // // fprintf(stderr, "PC %02x: unknown instructions %o2x\n", cpu->reg, instruction);
      // // exit(3);
      //   break;
    }
    cpu->PC += num_operands + 1;
    
    //    if (!num_operands) {
    //   cpu->PC += ((instruction >> 6) & 0x3) + 1;
    // } else {
    //   NULL;
    // }

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  // cpu->FL = ADDR_PROGRAM_ENTRY;
  cpu->FL = 0;
  cpu->SP = ADDR_EMPTY_STACK;
  // cpu->SP = 7;
  // cpu->reg[SP] = ADDR_EMPTY_STACK;

  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

// I KNOW WHAT I NEED TO DO I JUST DON'T KNOW HOW TO DO IT PLEASE SEND VIDS OR
// A GUIDE OF SOME KIND WHERE I MAY FIGURE IT OUT OR SOME ADVICE WHERE I CAN FINISH IT 
// THANK YOU ALEC