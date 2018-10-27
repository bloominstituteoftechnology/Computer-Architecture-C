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
    cpu->RAM[address++] = data[i];
  }
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   FILE *fp;
  //   int address = 0;
  //   char line[1024];

  //   // open file
  //   fp = fopen( filename, 'r');
  //   if (fp == NULL) {
  //     printf("Could not print file");
  //     return
  //   }

  //   while (fgets(line, sizeof line, fp) != NULL) {
  //     char *end_of_instr;
  //     unsigned char *data;

  //     data = strtol(line, &end_of_instr, 2);
  //   }
  //   if(){

  //   } else {
  //       cpu->ram[address++] = data[i];
  //   }
  //   // fscanf(fp, '%s', line);
    
  //   // if ( strchr(line, '#') != NULL) {
    
  //   // }
  // };
}

  // int address = 0;

  

  // TODO: Replace this with something less hard-coded


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
  int running = 1; // True until we get a HLT instruction

  // while (running) {
  //   // TODO
  //   // 1. Get the value of the current instruction (in address PC).
  //   // 2. switch() over it to decide on a course of action.
  //   // 3. Do whatever the instruction should do according to the spec.
  //   // 4. Move the PC to the next instruction.
    
  // }
  unsigned char test;
  test = cpu_ram_read(cpu, cpu->PC);
  printf("Here is the test: %u", test);
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