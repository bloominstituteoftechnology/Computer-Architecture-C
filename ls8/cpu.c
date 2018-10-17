#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 255
#define STACK_POINTER 7  // Define the register reserved for the stack pointer

// Helper functions
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

  printf("\nValue at register %d is: %d\n", regA, cpu->registers[regA]);
  return 1;
  
}

int handle_PUSH(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  cpu_ram_write(cpu, cpu->registers[STACK_POINTER]--, cpu->registers[regA]);
  return 1;
  
}

int handle_POP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  unsigned int stack_val = cpu_ram_read(cpu, ++cpu->registers[STACK_POINTER]); // stack pointer sits at empty slot for push, increment first to get last pushed item
  cpu->ram[cpu->registers[STACK_POINTER]] = 0;    // Set the value in ram (stack) to 0 (cuz popped)
  handle_LDI(cpu, regA, stack_val);               // Set the desired register = popped value
  return 1;

}

int handle_HALT(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  return 0;

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
    if(((binary_instruction >> 5) & 0x1) > 0)   // See if 00x00000 bit is 1 (indicating ALU instruction)
    {

      alu(cpu, binary_instruction, operandA, operandB);

    }
    else{
      // Finds the correct function in the array and calls it
      running = (*non_ALU_instructions[binary_instruction])(cpu, operandA, operandB); 

    }
    

    // 4. Move the PC to the next instruction. Add 1 to account for instruction argument
    int index_increment = (int) (binary_instruction >> 6) + 1;

    cpu->pc += index_increment;

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
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


}
