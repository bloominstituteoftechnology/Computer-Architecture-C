#include <stdio.h>  // install libraries
#include <stdlib.h>
#include <string.h> // for memset string library
#include "cpu.h"

#define DATA_LEN 6

// reading from the ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)  // helper function; return the address of the cpu
{
  return cpu->ram[address]; // return whatever the address of the ram; the cpu at this address will return the bytes that's there
}

//write to the ram
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)  // helper function; pass in a value since we are writing to the ram; return the address of the cpu and value
{
  cpu->ram[address] = value; 
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)  // cpu load taking in cpu and filename
{
  // FILE *fp = fopen(filename, "r");  // open a file; grab the pointer to the file and open it
  // printf("%s\n", filename);

  // char line[256]; // give a buffer or enough memory for each line to scroll the whole line; 

  // const int DATA_LEN = 6;

  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8; 1 operand
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0; 2 operand
    0b00000000,
    0b00000001  // HLT; 0 operand
  };

  int address = 0;  // starting address 

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
      cpu->reg[regA] *= cpu->reg[regB];  // multiply values in register A and register B and store value in RegA
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) 
  {
    // TODO
    
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->pc); // instruction register; pass in cpu and the program count er to read off that; read from RAM at the PC; this is the instruction/ opcode

    unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1); // openrandA will make 1 movement; reading at the next address of whatever the pc points at
    unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2); // openrandB will make 2 movements; reading the next byte and store in operand B


    // 2. switch() over it to decide on a course of action.
    // printf("TRACE: %02x: %02x\n", cpu->pc, IR);

    switch (IR) { // switch on the IR 

      case PRN: // call print function; print numeric value in the given register
        printf("%d\n", cpu->reg[operandA]); // print whatever is in operandA
        // printf("print 8 manually\n");        
        cpu->pc += 2;  // move the program counter by 2
        break;
      
      case LDI:   // LDI/load instruction
        // printf("We're about to load.\n");
        cpu->reg[operandA] = operandB; // operandA tells us the register number we need to store the value in; operandB is the value we need to store 
        cpu->pc += 3; // increment the pc; LDI is 3 bytes
        break;
      
      case HLT: // halt method
        // printf("This is the halt method.\n"); // does not move the program counter
        running = 0;  // then running is equal false or 0
        break;
      
      case MUL: 
        cpu->reg[operandA] *= cpu->reg[operandB]; // mutiplication 
        cpu->pc += 3; // increment the pc; LDI is 3 bytes      
        // alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case ADD: 
        cpu->reg[operandA] += cpu->reg[operandB]; // mutiplication 
        cpu->pc += 3; // increment the pc; LDI is 3 bytes      
        break;

      default:  // default case; if something wrong happes; will catch anything that it doesn't know what to do
        printf("Unknown instruction at %02x: %02x\n", cpu->pc, IR); // if we get instruction doesn't know, this will tell us where and what it is; tell it the pc counter which it's pointing at
        exit(2);  // exit with code 2
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;  // initialize/ load instructions at address 0

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof cpu->reg);  // grab the registers; we fill in the array with zeros
  memset(cpu->ram, 0, sizeof cpu->ram);  // grab the ram; we fill in the array with zeros
}
