#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
// Initilize int to keep track of length of program in RAM
// Used to trigger stack overflow warning
unsigned int loaded_ram_address = 0;

////////////////////
// Helper Functions
////////////////////
// cpu_ram_read
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// Implement cpu_ram_write
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val)
{
  cpu->ram[address] = val;
};

void push(struct cpu *cpu, unsigned char val)
{
  // Decrement stack pointer
  cpu->SP--;
  if (cpu->SP <= loaded_ram_address)
  {
    fprintf(stderr, "Stack overflow.\n");
    exit(4);
  }
  // Write to stack
  cpu_ram_write(cpu, cpu->SP, val);
}

void pop(struct cpu *cpu, unsigned char reg)
{
  // Get value at stack pointer
  unsigned char value = cpu_ram_read(cpu, cpu->SP);
  // Increment stack pointer
  cpu->SP++;
  if (cpu->SP > 244)
  {
    fprintf(stderr, "Stack underflow.\n");
    exit(4);
  }
  // assign value to given register
  cpu->reg[reg] = value;
}
///////////////////////
// Helper Functions End
///////////////////////

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  // Access file
  FILE *fp;
  // Initialize buffer to read line by line
  char line_buf[1024];
  // Initialize starting address
  loaded_ram_address = 0;
  // Check file exists
  if ((fp = fopen(file, "r")) == NULL)
  {
    fprintf(stderr, "File doesn't exist\n");
    exit(2);
  }

  // While line exists
  while (fgets(line_buf, sizeof(line_buf), fp) != NULL)
  {
    // initialize pointer to store string part of line
    char *ptr;
    // initialize int to store number part of line
    unsigned int ret;
    // convert string to number
    ret = strtol(line_buf, &ptr, 2);
    // printf("Line: %s\n", line_buf);
    // printf("The number part is %u\n", ret);
    // printf("String part is %s \n", ptr);

    // If string equal to line (ie. blank) go to next line
    if (ptr == line_buf)
    {
      continue;
    }
    // store converted number in ram and increment address
    cpu->ram[loaded_ram_address++] = ret;
  }
  // Close file
  fclose(fp);
}

/**
 * ALU
 */
// TODO: Find better implementation of ALU
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
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
  unsigned char operandA;
  unsigned char operandB;

  while (running)
  {
    // 1. Get the value of the current instruction (in address PC). Store in Instruction Register or IR
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Determine how many operands this next instruction requires from bits 6-7 of instruction opcode
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
      operandB = cpu_ram_read(cpu, cpu->PC + 2);
    }
    else if (num_operands == 1)
    {
      operandA = cpu_ram_read(cpu, cpu->PC + 1);
    }

    printf("TRACE: %02X: %02X   %02X %02X\n", cpu->PC, instruction, operandA, operandB);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (instruction)
    {
    case HLT:
      // Halt the CPU (and exit the emulator).
      running = 0;
      break;
    case LDI:
      // Set the value of a register to an integer.
      cpu->reg[operandA] = operandB;
      break;
    case MUL:
      // TODO: Find better implementation of ALU
      // *This is an instruction handled by the ALU.*
      // Multiply the values in two registers together and store the result in registerA.
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case POP:
      // Pop the value at the top of the stack into the given register.
      pop(cpu, operandA);
      break;
    case PRN:
      // Print numeric value stored in the given register.
      printf("%d\n", cpu->reg[operandA]);
      break;
    case PUSH:
      // Push the value in the given register on the stack.
      push(cpu, cpu->reg[operandA]);
      break;
    default:
      printf("unexpected instruction 0x%02X at 0x%02X\n", instruction, cpu->PC);
      exit(3);
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  //Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  // Empty stack starts at address F4
  cpu->SP = 0xF4;
}
