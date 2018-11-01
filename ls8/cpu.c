#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #define DATA_LEN 6
// #define DATA_LEN 50

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
  int instruction_counter = 0;

  file = fopen(filename, "r"); 

  if (file == NULL) {
    printf("Cannot read file."); 
    return;
  }

  while (RAM_INDEX < 256 && fgets(line, sizeof line, file) != NULL) {
    char *end_byte;
    unsigned char data = strtol(line, &end_byte, 2);

    if (data == *line) {
      instruction_counter++;
      continue;
    } else {
      cpu->RAM[RAM_INDEX++] = data;
    }
  }
  cpu->instruction_counter = instruction_counter;-
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
  unsigned char  IR, opA, opB, call;
  int RAM_INDEX; // track location of instruction in RAm
  int running = 1; // True until we get a HLT instruction

  RAM_INDEX = cpu->PC;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    // we need the next two bytes of instructions!!!!!!11
    IR = cpu_ram_read(cpu, RAM_INDEX);
    opA = cpu_ram_read(cpu, RAM_INDEX + 1);
    opB = cpu_ram_read(cpu, RAM_INDEX + 2);
    
    
    void multiply(int a, int b) {
      cpu->registers[a] *= cpu->registers[b];
    }

    void push(int a) {
      cpu->registers[7]--;
      if (cpu->RAM[cpu->registers[7]] == cpu->RAM[cpu->instruction_counter]) {
        printf("Stack Overflow!");
      } else {
        cpu->RAM[cpu->registers[7]] = cpu->registers[a];
      }
    }

    int pop(int a) {
      if (cpu->registers[7] == 0xF4) {
        return printf("Stack is already empty!");
      } else {
        cpu->registers[a] = cpu->RAM[cpu->registers[7]];
        cpu->registers[7]++;
      }
      return cpu->registers[a];
    }

    void add(int a, int b) {
      cpu->registers[a] += cpu->registers[b];
    }
    // let's do this...word to the trizzle
    switch(IR) {
        case LDI: {
            // 0b10000010 
            cpu->registers[opA] = opB;
            RAM_INDEX += 3;
            break;
        }
        case PRN: 
            printf("%d\n", cpu->registers[opA]);
            RAM_INDEX += 2;
            break;
        
        case ADD: 
            add(opA, opB);
            RAM_INDEX += 3;
            break;
        
        case RET: 
            pop(opA);
            RAM_INDEX += 1;
            break;
        
        case MUL: 
            multiply(opA, opB);
            RAM_INDEX += 3;
            break;
        
        case PUSH: 
            push(opA);
            RAM_INDEX += 2;
            break;
        
        case POP: 
            pop(opA);
            RAM_INDEX += 2;
            break;

        case HLT:
            running = 0;
            break;
        
        default:  
            printf("You cain't code fool!");
            running = 0;
    }
    RAM_INDEX += 2;
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