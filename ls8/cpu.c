#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DATA_LEN 6

// Implement CPU_RAM_READ & CPU_RAM_WRITE


// ------------------------ DAY 1 Helper Funcs ----------------------
// 1. Take in the cpu || an index to write into || and a value to write into it
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char val) {
    // 2. Set given index to the value given
    cpu->ram[index] = val;
}

// 1. Take in a cpu || index they want to read
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  return cpu->ram[index]; // cpu->ram[cpu->pc]
}
// --------------------------------------------------------------------




// ------------------------ DAY 2 Helper Funcs ----------------------

// --------------------------------------------------------------------

/*

  index:
  1. 0b10000010
  2. 
  3.
  ...
  256.

*/

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *argv[])
{ 
  printf("cpu_load print\n");
  // 1. BASE CASE: If invalid number of arguments are passed, error
  if (argc < 2) {
    fprintf(stderr, "Usage Error: Needs two arguments\n");
    printf("Argument 1: ./ls8\n");
    printf("Argument 2: examples/ls8_file_name.ls8\n");
    exit(1);
  }

  // 2. Access FILE, take note of target file, line buffer needed
  FILE *fp;
  char *file_name = argv[1];
  char line_buffer[1024];  
  int address_index = 0;

  // 3. BASE CASE 2: Error Opening File ( invalid file )
  if ( (fp = fopen(file_name, "r")) == NULL ) {
   fprintf(stderr, "Opening File Error: invalid file name\n");
   exit(1);
  }

  // 4. Read lines
  while ( fgets(line_buffer, sizeof(line_buffer), fp) != NULL ) {  
    // printf("%s\n", line_buffer);
    char *ptr;
    unsigned char ret;
    ret = strtol(line_buffer, &ptr, 2);

    // 4a. Skip over the empty lines / null lines
    if (ptr == line_buffer) {
      continue;
    }

    // 5. Store into RAM, increment address_index
    cpu->ram[++address_index] = ret;


  }   
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  printf("cpu_run print\n");
  int running = 1; // True until we get a HLT instruction
  unsigned char operand_a;
  unsigned char operand_b;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    unsigned int num_of_operands = instruction >> 6; // --> instruction >> 6
    // --> 0000011 --> 00000011
    // 3. Get the appropriate value(s) of the operands following this instruction          
      // --> 5. Do whatever the instruction should do according to the spec.
    if (num_of_operands == 2) {
      operand_a = cpu_ram_read( cpu, (cpu->PC + 1) & 0xff );
      operand_b = cpu_ram_read( cpu, (cpu->PC + 2) & 0xff );      
    } else if (num_of_operands == 1) {
      operand_a = cpu_ram_read( cpu, (cpu->PC + 1) & 0xff );      
    } else {      
    }

    // --> 4. switch() over it to decide on a course of action.
    switch(instruction) {
      case HLT:
        running = 0;
        break;
      
      case LDI:        
        cpu->registers[operand_a] = operand_b;        
        break;

      case PRN:
        printf("%d\n", cpu->registers[operand_a]);        
        break;

      case MUL:
        alu(cpu, ALU_MUL, operand_a, operand_b);
        break;
      
      default:
        break;
    } // --> switch()

    // 6. Move the PC to the next instruction.   
    cpu->PC += num_of_operands + 1; 
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers  
  printf("cpu_init print\n");
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
