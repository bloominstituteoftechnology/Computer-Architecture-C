#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DATA_LEN 6

// returns the byte we are reading from the cpu struct, with the address = memory address register (mar) 
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar) {
  return cpu->ram[mar];
}
// mdr = memory data register
void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr) {
  cpu->ram[mar] = mdr;
}
// "read" returns the value, "write" sets the value in the array.

// Establishing Stack functions
// SP = Stack Pointer
// push onto the stack we are going to decrement, the address is going to be moving toward 0 in ram
void cpu_push(struct cpu *cpu, unsigned char value) {
  cpu->SP--;
  cpu_ram_write(cpu, value, cpu->SP); 
}
// pop onto the stack we are going to increment
unsigned char cpu_pop(struct cpu *cpu) {
  return cpu_ram_read(cpu, cpu->SP++);
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  ////// This is what we started with....
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };
  // int address = 0;
  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }
  ///////

  // TODO: Replace this with something less hard-coded

  //// This wouldn't stop running....
  // FILE *fp;
  // // 1024 = char limit
  // char line[1024];
  // // index
  // int address = ADDR_PROGRAM_ENTRY;

  // // open the source file, r must be in "" not '', single = character, double = char pointer
  // if ((fp = fopen(filename, "r")) == NULL) {
  //   // error handling
  //   fprintf(stderr, "Can NOT open file %s\n", filename);
  //   exit(2);
  // }
  // // read all the lines and store them in RAM
  // while (fgets(line, sizeof line, fp) !=NULL) {
  //   // convert string to number(long int) = strol
  //   char *endchar;
  //   unsigned char byte = strtol(line, &endchar, 2);
  //   // no numbers = don't read
  //   // line is the first character, endchar is the first none/invalid character/the end
  //   if (endchar == line) {
  //     // which means there are no numbers
  //     continue;
  //   }
  //   // write to the RAM, storage
  //   cpu_ram_write(cpu, byte, address++);
  // }
  // fclose(fp);
  //////////

  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr = 0x00; //memory init to 0
  // error handling
  if (fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2); // <stdlib.h>
  }
  // while not = to null, read the lines from the file and store to memory
  while (fgets(line, sizeof line, fp) != NULL) {
    // a pointer to a pointer, so we can change the original pointer
    char *endpointer;
    // The strtoul() function converts a character string to an unsigned long integer value.
    unsigned char bytes = strtoul(line, &endpointer, 2);
    if (endpointer == line) {
      // which means there are no numbers
      continue;
    }
    // store results to memory
    cpu_ram_write(cpu, addr++, bytes);
  }
  // close the file
  fclose(fp);
}

/**
 * ALU
 */
// 
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
      break;
    
    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
      break;
    
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  // declare instruction run variable, operandA and operandB are bytes PC1 and PC2
  unsigned char IR, operandA, operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // bring in the read function, get the instruction at that address
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.

    // PC = index of current executing in struction in RAM array, +1,+2 are the next index after
    // & 0xff = ??? suppose to help with a bug
    IR = cpu_ram_read(cpu, cpu->PC);
    operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
    
    int add_to_pc = (IR >> 6) + 1;

    // printing it out: 02X =lead with zeros, 2 digits wide, x means capital lettering
    printf("TRACE:  %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);

    switch(IR) {
      //
      case LDI:
        // register number 0, set to the value 8
        cpu->reg[operandA] = operandB;
        // moves the PC to the next instruction
        // cpu->PC += 3;
        break;

      // PRN- print out the numeric value in a register
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        // moves the PC to the next instruction
        // cpu->PC += 2;
        break;

      // Multiply
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      // Halt
      case HLT:
        // if running is false then break
        running = 0;
        // moves the PC to the next instruction
        // cpu->PC += 1;
        break;

    }
    // moves the PC to the next instruction
    cpu->PC += add_to_pc;
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // start at address 0
  cpu->PC = 0;
  // initialize the Stack Pointer = address of empty stack
  cpu->SP = ADDR_EMPTY_STACK;
  // zero registers and RAM, memeset = is used to fill a block of memory w/ a particular value: GeeksforGeeks
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->ram, 0, sizeof cpu->reg);
}
