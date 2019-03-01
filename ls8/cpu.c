#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Define which register to store Stack Pointer
#define SP_REG 7
// Define which register handles interrupt status
#define IS_REG 6
// Define which register handles interrupt mask
#define IM_REG 5

// Initilize int to keep track of length of program in RAM
// Used to trigger stack overflow warning
unsigned int loaded_ram_address = 0;

////////////////////
// Helper Functions
////////////////////

// Return value at RAM address
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// Write value at RAM address
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val)
{
  cpu->ram[address] = val;
};

// Push value to top of stack
void push(struct cpu *cpu, unsigned char val)
{
  // Decrement stack pointer and check for overflow
  if (cpu->reg[SP_REG]-- <= loaded_ram_address)
  {
    fprintf(stderr, "Stack overflow.\n");
    exit(4);
  }
  // Write to stack
  cpu_ram_write(cpu, cpu->reg[SP_REG], val);
}

// Return value at top of stack
unsigned char pop(struct cpu *cpu)
{
  // Get value at stack pointer and increment
  unsigned char value = cpu_ram_read(cpu, cpu->reg[SP_REG]++);
  // Check if stack is empty
  if (cpu->reg[SP_REG] > 244)
  {
    fprintf(stderr, "Stack underflow.\n");
    exit(4);
  }
  // return value
  return value;
}

void handle_interrupt(struct cpu *cpu, int interrupt_flag)
{
  // 1. The IM register is bitwise AND-ed with the IS register and the results stored as `maskedInterrupts`.
  unsigned int maskedInterrupts = cpu->reg[IS_REG] & cpu->reg[IM_REG];
  unsigned int check_bit = 0x01;
  // 2. Each bit of `maskedInterrupts` is checked, starting from 0 and going up to the 7th bit, one for each interrupt.
  for (int i = 0; i < 8; i++)
  {
    // If interrupt found
    if (maskedInterrupts & check_bit)
    {
      // 1. Change interrupt flag - disabling interrupts until IRET
      interrupt_flag = 1;
      // 2. Clear bit in the IS register
      cpu->reg[IS_REG] = cpu->reg[IS_REG] ^ check_bit;
      // 3. Push PC on stack
      push(cpu, cpu->PC);
      // 4. Push FL register
      // push(cpu, cpu->FL);
      // 5. Push registers 0-6 in order
      for (int j = 0; j < 7; j++)
      {
        push(cpu, cpu->reg[j]);
      }
      // 6. The address (_vector_ in interrupt terminology) of the appropriate handler is looked up from the interrupt vector table.
      unsigned int vector = 0b11111000 | i;
      // 7. Set the PC is set to the handler address.
      cpu->PC = cpu_ram_read(cpu, vector);
      // break for loop at bit handling first interrupt
      break;
    }
    // else shift bit to check next one
    check_bit = check_bit << 1;
  }
  // return to cpu_run
}

void handle_IRET(struct cpu *cpu, int interrupt_flag)
{
  for (int i = 6; i >= 0; i--)
  {
    cpu->reg[i] = pop(cpu);
  }
  // cpu->FL = pop(cpu);
  cpu->PC = pop(cpu);
  interrupt_flag = 0;
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
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;
  case ALU_CMP:
    // Set FL to 0
    cpu->FL = cpu->FL & 0x00;
    // if A less than B set flag L 00000100 = 4
    if (cpu->reg[regA] < cpu->reg[regB])
    {
      cpu->FL = cpu->FL | 4;
    }
    // if A greater than B set flag G 00000010 = 2
    else if (cpu->reg[regA] > cpu->reg[regB])
    {
      cpu->FL = cpu->FL | 2;
    }
    // else if equal set flag E 00000001 = 1
    else
    {
      cpu->FL = cpu->FL | 1;
    }
    break;
  case ALU_MUL:
    cpu->reg[regA] *= cpu->reg[regB];
    break;
  default:
    break;
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
  struct timeval tv;
  gettimeofday(&tv, NULL);
  unsigned int prev_sec = tv.tv_sec;
  int interrupt_flag = 0;

  while (running)
  {
    // 1. Get the value of the current instruction (in address PC). Store in Instruction Register or IR
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Determine how many operands this next instruction requires from bits 6-7 of instruction opcode
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->PC + 1);
    operandB = cpu_ram_read(cpu, cpu->PC + 2);

    // Check for time interrupt
    gettimeofday(&tv, NULL);
    // printf("tv: %ld  prev_sec: %u\n", tv.tv_sec, prev_sec);
    if (tv.tv_sec != prev_sec)
    {
      cpu->reg[IS_REG] |= 0x01;
    }
    prev_sec = tv.tv_sec;

    // printf("TRACE: %02X: %02X   %02X %02X\n", cpu->PC, instruction, operandA, operandB);

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (instruction)
    {
    case ADD:
      // *This is an instruction handled by the ALU.*
      // Add the value in two registers and store the result in registerA.
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case CALL:
      // Calls a subroutine (function) at the address stored in the register.
      // 1. Store address of next instruction after CALL on stack
      push(cpu, (cpu->PC + num_operands + 1));
      // 2. PC is set to address stored in given register
      cpu->PC = cpu->reg[operandA];
      break;
    case CMP:
      // *This is an instruction handled by the ALU.*
      // Compare the values in two registers.
      alu(cpu, ALU_CMP, operandA, operandB);
      break;
    case HLT:
      // Halt the CPU (and exit the emulator).
      running = 0;
      break;
    case IRET:
      // Return from an interrupt handler.
      handle_IRET(cpu, interrupt_flag);
      break;
    case JEQ:
      // If `equal` flag is set (true), jump to the address stored in the given register.
      if (cpu->FL & 1)
      {
        cpu->PC = cpu->reg[operandA];
      }
      // Else manually increment PC if it isn't set
      else
      {
        cpu->PC += num_operands + 1;
      }
      break;
    case JMP:
      // Jump to the address stored in the given register.
      // Set the `PC` to the address stored in the given register.
      cpu->PC = cpu->reg[operandA];
      break;
    case LD:
      // Loads registerA with the value at the memory address stored in registerB.
      cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[operandB]);
      break;
    case LDI:
      // Set the value of a register to an integer.
      cpu->reg[operandA] = operandB;
      break;
    case MUL:
      // *This is an instruction handled by the ALU.*
      // Multiply the values in two registers together and store the result in registerA.
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case POP:
      // Pop the value at the top of the stack into the given register.
      cpu->reg[operandA] = pop(cpu);
      break;
    case PRA:
      // Print alpha character value stored in the given register.
      printf("%c\n", cpu->reg[operandA]);
      break;
    case PRN:
      // Print numeric value stored in the given register.
      printf("%d\n", cpu->reg[operandA]);
      break;
    case PUSH:
      // Push the value in the given register on the stack.
      push(cpu, cpu->reg[operandA]);
      break;
    case RET:
      // Return from subroutine.
      // Pop the value from the top of the stack and store it in the `PC`.
      cpu->PC = pop(cpu);
      break;
    case ST:
      // Store value in registerB in the address stored in registerA.
      cpu_ram_write(cpu, cpu->reg[operandA], cpu->reg[operandB]);
      break;
    default:
      printf("unexpected instruction 0x%02X at 0x%02X\n", instruction, cpu->PC);
      exit(3);
    }
    // 6. Check to see if instruction sets PC. Move the PC to the next instruction if needed.
    int PC_set = instruction >> 4 & 0x01;
    if (PC_set == 0)
    {
      cpu->PC += num_operands + 1;
    }
    // 7. Check for interrupt and check if not already handling an interrupt
    if ((cpu->reg[IS_REG] != 0) && interrupt_flag == 0)
    {
      handle_interrupt(cpu, interrupt_flag);
    }
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
  cpu->reg[SP_REG] = 0xF4;
}
