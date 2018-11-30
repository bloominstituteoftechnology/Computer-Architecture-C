#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DATA_LEN 6


// CPU Ram read
unsigned char cpu_ram_read(unsigned char index, struct cpu *cpu) 
{
  return cpu->ram[index];
}
// CPU Ram write
void cpu_ram_write(unsigned char index, struct cpu *cpu, unsigned char value) 
{
  cpu->ram[index] = value;
}




/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp = fopen(argv[1], "r"); // where is path defined? at function call
  char str[256];
  int address = 0;
  // char *ptr;
  // char *file;
  if(fp == NULL) {
    printf("error reading the file");
  }
  while(fgets(str, sizeof(str), fp)) {
    unsigned char ret = strtoul(str, NULL, 2);
    cpu_ram_write(address++, cpu, ret);
  }
  fclose(fp);
}


/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->registers;
  switch (op) {
    case ALU_MUL:
      reg[regA] *= reg[regB];
      break;
    case ALU_ADD:
      reg[regA] += reg[regB];
      break;
    case CMP:
      if(reg[regA] == reg[regB]){
        
      }else if (reg[regA] < reg[regB]){

      }
    // TODO: implement more ALU ops
  }
}


/**
 * Run the CPU */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char ir;
  unsigned char register_index;
  int num;

  
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    ir = cpu_ram_read(cpu->pc, cpu);
    unsigned char operA = cpu_ram_read(cpu->pc + 1, cpu);
    unsigned char operB = cpu_ram_read(cpu->pc + 2, cpu);
    // 2. switch() over it to decide on a course of action.
    switch(ir) {
      case HLT: // HLT
        running = 0;
        break;
      case PRN:
        register_index = cpu_ram_read(cpu->pc + 1, cpu);
        printf("register value %d\n", cpu->registers[register_index]);
        cpu->pc += 2;
        break;
      case LDI:   
        num = cpu_ram_read(cpu->pc + 2, cpu); 
        cpu->registers[cpu_ram_read(cpu->pc + 1, cpu)] = num;
        cpu->pc += 3;
        break; 
      case ALU_MUL:
        alu(cpu, ALU_MUL, operA, operB);
        cpu->pc += 3;
        break;
      // need to create a case for stack DS
      case PUSH: 
        // push function
        cpu->registers[7]--;
        cpu_ram_write(cpu->registers[7],cpu, cpu->registers[operA]);   
        cpu->pc += 2; 
        break;
      case POP:
        if(cpu->registers[7] == 0xF4){
          fprintf(stderr,"error can not pop");
        }
        cpu->registers[operA] = cpu_ram_read(cpu->registers[7],cpu);
        cpu->registers[7]++;
        cpu->pc += 2;
        break;
      case CALL:
        // if(cpu->pc[str])
        cpu->registers[7]--;
        cpu->ram[cpu->registers[7]] = cpu->pc + 2;
        cpu->pc = cpu->registers[operA];
        break;
      case RET:
        cpu->pc = cpu->ram[cpu->registers[7]];
        cpu->registers[7]++;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operA, operB);
        cpu->pc += 3;
        break;
      case CMP:


    }
   }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // cpu = calloc(1, sizeof(struct cpu));
  // TODO: Zero registers and RAM
  cpu->pc = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
  cpu->registers[7] = 0xF4; 
  
}
