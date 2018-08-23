#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG 1

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{

  // Original code that I used for print8:
  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8  /* instruction */
  //   0b00000000, /* argument 1 */
  //   0b00001000, /* argument 2 */
  //   0b01000111, // PRN R0 /* instruction */
  //   0b00000000, /* argument 1 */
  //   0b00000001  // HLT /* instruction */
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *fp; // file pointer
  char line[256]; // storage for line of file
  int counter = 0; // way to track index in ram

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(1);
  }
    #if DEBUG
     printf("\n**********Lines from file:***********\n");
     #endif

    while (fgets(line, sizeof(line), fp) != NULL) { // read line from file and store in line up to 256 bytes.
      char *ptr;
      unsigned char byte = strtoul(line, &ptr, 2);
      if (ptr == line) {
        continue;
      }
      cpu->ram[counter++] = byte; // converts string to unsigned long integer using base 2 and stores in ram
      
      #if DEBUG
      printf("Value of line: %s", line);
      #endif
    }
   
    #if DEBUG
    printf("\n*******RAM in Load*******\n");
    for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\n", i, cpu->ram[i]);
    }
    #endif
  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}


/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valB = reg[regB];
  switch (op) {
    case ALU_ADD:
      reg[regA] += valB;
      break;
    // case ALU_AND:
    //   regA = regA & regB;
    //   break;
    // case ALU_DEC:
    //   regA = regA - 1;
    //   break;
    // case ALU_SUB:
    //   regA = regA - regB;
    //   break;
    case ALU_MUL:
      reg[regA] *= valB;
      break;
    // TODO: implement more ALU ops
  }
}

// void push(struct cpu *cpu, unsigned char regA)
// {
//   cpu_ram_write(cpu, --cpu->reg[7], regA);
// }

// unsigned char pop(struct cpu *cpu)
// {
//   unsigned char popped = cpu_ram_read(cpu, cpu->reg[SP++])
//   return popped;
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  // unsigned char PC = cpu->PC;
  int running = 1; // True until we get a HLT instruction
  
  while (running) {
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    int difference = ((IR >> 6) & 0b11) + 1; // shifts the number 6 places to the right (leaving last two places)
    // since the number of operands can be found in the two high bits, add one for opcode to get to next instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);

    switch(IR)
    {
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC+=difference;
        break;
      case LDI:
        reg[operandA] = operandB;
        cpu->PC+=difference;
        break;
      case PRN:
        printf("%d\n", reg[operandA]);
        cpu->PC+=difference;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC+=difference;
        break;
      case POP:
        reg[operandA] = cpu->ram[cpu->reg[SP]]++;
        cpu->PC+=difference;
        break;
      case PUSH:
        cpu_ram_write(cpu, --cpu->reg[SP], reg[operandA]);
        cpu->PC+=difference;
        break;
      case HLT:
        running = 0;
        break;
      case RET:
        #if DEBUG
        printf("Value of PC before RET: %d\n", cpu->PC);
        #endif
        cpu->PC = cpu->ram[cpu->reg[SP]]++;
        #if DEBUG
        printf("Value of PC after RET: %d\n", cpu->PC);
        #endif
        break;
      case ST:
        cpu->ram[reg[operandA]] = reg[operandB];
        cpu->PC+= difference;
        break;
      case CALL:
        #if DEBUG
        printf("Value before call: %d\n", cpu->PC);
        #endif
        cpu_ram_write(cpu, --cpu->reg[SP], cpu->PC+=difference);
        #if DEBUG
        printf("Value of PC in register: %d\n", cpu->PC);
        #endif
        cpu->PC = cpu->reg[operandA];
        #if DEBUG
        printf("Value of PC after call: %d\n", cpu->PC);
        #endif
        break;
      case PRA:
        printf("%c\n", reg[operandA]);
        cpu->PC+=difference;
        break;
      case IRET:
        reg[6] = cpu->ram[cpu->reg[SP]]++;
        reg[5] = cpu->ram[cpu->reg[SP]]++;
        reg[4] = cpu->ram[cpu->reg[SP]]++;
        reg[3] = cpu->ram[cpu->reg[SP]]++;
        reg[2] = cpu->ram[cpu->reg[SP]]++;
        reg[1] = cpu->ram[cpu->reg[SP]]++;
        reg[0] = cpu->ram[cpu->reg[SP]]++;
        // cpu->FL = cpu->ram[SP++];
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;
      default:
        printf("Unknown instruction at %02x: %02x\n", cpu->PC, IR);
        exit(2);
    }

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
  #if DEBUG
    printf("\n********RAM in run********\n");
      for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\n", i, cpu->ram[i]);
    }
    printf("\n*******End of debugger********\n\n");
  #endif
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->reg = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
  cpu->reg[7] = 0xF4;
  cpu->reg[6] = 00000000; // IS register - interrupt status
  cpu->reg[5] = 00000000; // interrupt mask (change to 1 to start checking for interrupt)
  cpu->FL = 00000000;
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
