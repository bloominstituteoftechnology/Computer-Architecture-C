#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"

// Beej
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}



/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  char line[1024];
  int address = 0;

  FILE *fp = fopen(filename, "r");

  while(fgets(line, sizeof line, fp) != NULL) {
    char *endchar;
    unsigned char v = strtoul(line, &endchar, 2);

    if(line == endchar) {
      continue;
    }

    cpu_ram_write(cpu, address++, v);
  }



  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8   // instruction
  //   0b00000000, // operand 1
  //   0b00001000, // operand 2
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

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
    default:
      printf("Can't read that operand");

    // TODO: implement more ALU ops
    // case ALU_ADD:
    //   cpu->reg[regA] += cpu->reg[regB];
    //   break;
  }
}


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int address;
  unsigned char IR;
  int IR_size;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    //=========== part 1 solution ==============


    // unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // unsigned char operandA = cpu_ram_read(cpu, cpu->PC +1);
    // unsigned char operandB = cpu_ram_read(cpu, cpu->PC +2);
    // printf("TRACE: %02x: %02x\n", cpu->PC, IR);

    // ============ part 1 end solution =============

    address = cpu->PC;
    IR = cpu_ram_read(cpu, address);
    unsigned char operandA = cpu_ram_read(cpu, address +1);
    unsigned char operandB = cpu_ram_read(cpu, address +2);
    IR_size = (IR >> 6) +1;

    switch(IR) {
      case LDI:
        cpu->reg[operandA] = operandB;
        // cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        // cpu->PC += 2;
        break;
      case HLT:
        running = 0;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case PUSH:
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
        break;
      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]);
        cpu->reg[SP] ++;
        break;
      default:
        printf("unknown in Instructions");
    }
    cpu->PC += IR_size;
  }
}



// void read_file(char *filename) {
//   char array[200];
//   int counter = 0;
//   FILE * fPointer;
//   fPointer = fopen(filename, "r");

//   while(!feof(fPointer)) {
//     // counter ++;
//     fgets(array, 200, fPointer);
//     puts(array);
//     counter++;
//   }
//   printf("counter is %d\n", counter);
//   fclose(fPointer);
// }

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC=0;
  cpu->reg[SP] = 0xf4;
  // read_file("examples/mult.ls8");
  // TODO: Zero registers and RAM
}

