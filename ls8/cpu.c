#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 255
#define STACK_POINTER 7  // Define the register reserved for the stack pointer

// Instruction handlers
unsigned char cpu_ram_read(struct cpu *cpu, int mar)
{

  return cpu->ram[mar]; //mar = memory address register

}

void cpu_ram_write(struct cpu *cpu, int mar, unsigned char value)
{

  cpu->ram[mar] = value;
  
}

int handle_LDI(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  cpu->registers[regA] = regB;
  return 1;
  
}

int handle_PRN(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  printf("\nValue at register %d is: %d\n", regA, cpu->registers[regA]);
  return 1;
  
}

int handle_PUSH(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->registers[regA]); // Decrement stack pointer first, then write value in register A to stack (according to spec)
  return 1;
  
}

int handle_POP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  unsigned int stack_val = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Get the value at stack pointer, then increment
  handle_LDI(cpu, regA, stack_val);               // Set the desired register = popped value
  return 1;

}

int handle_HALT(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)cpu;
  (void)regA;
  (void)regB;
  return 0;

}

int handle_CALL(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  // Push address of next instruction to the stack
  cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->pc + 2);
  return cpu->registers[regA];                                            // return the value in register A (the pc value of the called function)

}

int handle_RET(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regA;
  (void)regB;
  // Pop pc address of next instruction off of the stack
  unsigned int return_pc = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Get the value at stack pointer, then increment
  return return_pc;                                            // return the pc value of the address just after the previous call

}

// Declare an array of pointers to functions, add functions to array in cpu_init
int (*non_ALU_instructions[DATA_LEN]) (struct cpu * cpu, unsigned char regA, unsigned char regB);



/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char* fileName)
{
  char data[DATA_LEN];

  FILE* file_ptr;

  if((file_ptr = fopen(fileName, "r")) == NULL)
  {
    printf("Error opening file!\nPlease include a relative path with filename\n");
    exit(1);
  }

  int i = 0;
  while(fgets(data, sizeof(data), file_ptr) != NULL)
  {
    if(data[0] == '\n' || data[0] == '#')
    {
      continue;
    }

  
    unsigned char num;
    num = strtoul(data, NULL, 2);

    cpu_ram_write(cpu, i, num);     // Write binary instructions to ram

    i++;
  }
  
  fclose(file_ptr);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, unsigned char op, unsigned char regA, unsigned char regB)
{
  switch (op) 
  {
      case MUL:
      {
        unsigned char product = cpu->registers[regA] * cpu->registers[regB];
        cpu->registers[regA] = product;
        break;
      }
      case ADD:
      {
        unsigned char sum = cpu->registers[regA] + cpu->registers[regB];
        cpu->registers[regA] = sum;
        break;
      }
        
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

  while (running) {

    // 1. Get the value of the current instruction (in address PC).
    int pc = cpu->pc;
    unsigned char binary_instruction = cpu_ram_read(cpu, pc);
    unsigned char operandA = cpu_ram_read(cpu, pc + 1);
    unsigned char operandB = cpu_ram_read(cpu, pc + 2);
    
    // 3. Do whatever the instruction should do according to the spec.
    
    if(((binary_instruction >> 4) & 0x1) == 1)      // If we have an instruction which changes the PC
    {

      cpu->pc = (*non_ALU_instructions[binary_instruction])(cpu, operandA, operandB);

    }
    else if(((binary_instruction >> 5) & 0x1) == 1)   // See if 00x00000 bit is 1 (indicating ALU instruction)
    {

      alu(cpu, binary_instruction, operandA, operandB);
      int index_increment = (int) (binary_instruction >> 6) + 1;
      cpu->pc += index_increment;

    }
    else{

      // Finds the correct function in the array and calls it
      running = (*non_ALU_instructions[binary_instruction])(cpu, operandA, operandB); 
      int index_increment = (int) (binary_instruction >> 6) + 1;
      cpu->pc += index_increment;

    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Place in ram where stack memory begins, decrements downwards
  int BEGIN_STACK = 0xF4;
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  // TODO: Zero registers and RAM
  
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);

  cpu->registers[STACK_POINTER] = BEGIN_STACK;

  // Assign our functions to the array of function handlers
  non_ALU_instructions[LDI] = handle_LDI;
  non_ALU_instructions[PRN] = handle_PRN;
  non_ALU_instructions[PUSH] = handle_PUSH;
  non_ALU_instructions[POP] = handle_POP;
  non_ALU_instructions[HLT] = handle_HALT;
  non_ALU_instructions[CALL] = handle_CALL;
  non_ALU_instructions[RET] = handle_RET;


}
