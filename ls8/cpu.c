#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar) {
  return cpu->RAM[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr) {
  // mar = memory address register
  // memory data registesr
  cpu->RAM[mar] = mdr;
}


void cpu_load(char *filename, struct cpu *cpu)
{
  int RAM_INDEX = 0; 
  FILE *file; 
  char line[1024];

  file = fopen(filename, "r"); 

  if (file == NULL) {
    printf("Cannot read file."); 
    return;
  }

  while (fgets(line, sizeof line, file) != NULL) { 
    char *end_of_byte;
    unsigned char data = strtol(line, &end_of_byte, 2);

    if (data == *line) {
      continue;
    }
    else {
      cpu->RAM[RAM_INDEX++] = data;
    }
  }
  
  fclose(file);
}

  

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char opA, opB;
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    // we need the next two bytes of instructions!!!!!!11
    opA = cpu_ram_read(cpu, cpu->PC + 1);
    opB = cpu_ram_read(cpu, cpu->PC + 2);
    unsigned char current_instruction = cpu->RAM[cpu->PC];
    
    // let's do this...word to the trizzle
    switch(current_instruction) {
        case LDI:
            // 0b10000010 
            cpu->registers[opA] = opB;
            cpu->PC += 3;
            break;
        case PRN:
            printf("%d\n", cpu->registers[opA]);
            cpu->PC+= 2;
            break;
        case HLT:
            running = 0;
            break;

    default:
         printf("You cain't code fool!");
         running = 0;
         break;
    
    }
  }
  // unsigned char test;
  // test = cpu_ram_read(cpu, cpu->PC);
  // printf("Here is the test: %u", test);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->RAM, 0, sizeof(cpu->RAM));
}