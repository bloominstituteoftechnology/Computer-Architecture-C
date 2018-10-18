#include "cpu.h"

#define DATA_LEN 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * HELPER FUNCTIONS
 *  cpu_ram_read()
 *  cpu_ram_write()
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char memory_ADDRESS_registry)
{
  return cpu->ram[memory_ADDRESS_registry];
}

void cpu_ram_write(struct cpu *cpu, unsigned char memory_ADDRESS_registry, unsigned char memory_DATA_registry)
{
  cpu->ram[memory_ADDRESS_registry] = memory_DATA_registry;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
/*
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
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
  }
}
*/
// TODO: Replace above with something less hard-coded
void cpu_load(char *filename, struct cpu *cpu)
{
  FILE *fp;
  char line[1024];
  // below, 0x00 initializes to zero
  unsigned char address = 0x00;
  // Open the source file
  // below, has to be "r", not 'r'
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s/n", filename);
    exit(2);
  }
  // Read all the lines and store them in RAM
  while (fgets(line, sizeof line, fp) != NULL) {
    // Convert string to num
    char *endpointer;
    unsigned char byte = strtoul(line, &endpointer, 2);
    // Ignore lines from which no numbers were read
    if (endpointer == line) {
      // printf("Ignoring: %s", line);
      continue;
    }
    // Store in RAM
      // ok below, but used helper function instead
    // cpu->ram[address++] = byte;

    cpu_ram_write(cpu, address++, byte);
  }
  fclose(fp);
} // <-- END OF cpu_load() -->

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  /*
  // below, 3 lines to disable compiler warnings (temp, until impl)
  (void)cpu;
  (void)regA;
  (void)regB;
  //
  */
  switch (op) {
    case ALU_MUL:
      // TODO
        // Multiply the values in two registers together and store the result in registerA.
      //below, to shorthand
      // cpu->reg[regA] += cpu->reg[regB];
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
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
    // TODO
    // 1. Get the value of the current instruction (in address PC).
       // `IR`: Instruction Register, contains a copy of the currently executing instruction
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);


     // True if this instruction might set the PC
    // int instruction_set_pc = (IR >> 4) & 1;
    int instruction_set_pc = (IR >> 6) + 1;

    printf("TRACE: --> pc:%02X | ir:%02X | opA:%02X | opB:%02X | reg:", cpu->PC, IR, operandA, operandB);
    for (int i = 0; i < 8; i++) {
      printf(" %02X", cpu->reg[i]);
    }
    printf(" |\n");


    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.

    switch(IR)
    {
       // `LDI`: load "immediate", store a value in a register, or "set this register to.
      case LDI:
        cpu->reg[operandA] = operandB;
        break;

       // `PRN`: a pseudo-instruction that prints the numeric value stored in a register.
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

       // `HLT`: halt the CPU and exit the emulator.
      case HLT:
        running = 0;
        break;

      // `MUL`: Multiply the values in two registers together and store the result in registerA.
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;


      default:
        fprintf(stderr, "ERROR: Unknown Instruction\n from cpu_run() in cpu.c\n PC: %02x\n IR: %02X\n", cpu->PC, IR);
        exit(3);
    } // <-- END OF switch -->


    // 4. Move the PC to the next instruction.

       // `>>` (right shift) Takes two numbers, right shifts the bits of the first operand, the second operand decides the number of places to shift.
       // `&` (bitwise AND) Takes two numbers as operands and does AND on every bit of two numbers. The result of AND is 1 only if both bits are 1.
       // https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/

    // if (!instruction_set_pc)
    // {
    //   cpu->PC += ((IR >> 6) & 0x3) +1;
    // }
    cpu->PC += instruction_set_pc;

  } // <-- END OF while loop -->
} // <-- END OF cpu_run() -->

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
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

