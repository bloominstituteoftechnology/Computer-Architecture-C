// header file to make access to CPU/ALU functions:
#include "cpu.h"
// include other stuff:
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  // unsigned because we need a return for the read
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value){
  // assign value to array element at given index.
  cpu->ram[index] = value;
}

// add args to cpuload to use FILE I/O:
void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  // make data into a big array
  char data[1024];
  //initialize memory address:
  unsigned char address = 0;
  //open file as read only to input instructions
  fp = fopen(argv[1], "r");
  //check if the file doesn't exist.
  if (fp == NULL){
    perror("file can't be opened");
    exit(1);
  }

  while(fgets(data, sizeof(data), fp) != NULL){
    //convert data to long unsigned integer
    unsigned char byte = strtoul(data, NULL, 2);

    if(data == NULL){
      continue;
    }
    cpu->ram[address++] = byte;
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
      // notice this overwrites whatevr registeer regA is as the product:
      regist[regA] *= regist[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
      regist[regA] += regist[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_SUB:
      regist[regA] -= regist[regB];
      break;
      // TODO: implement more ALU ops
    case ALU_DIV:
      regist[regA] /= regist[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_MOD:
      regist[regA] = regist[regA]%regist[regB];
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
    int c = ram[PC];
    switch (c) {
      case HLT:
      running = 0;
      break;
    }
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0x00;
  cpu->regist[SP] = 0xF4;
  //cpu->ram = calloc(256, sizeof(unsigned char));
  // TODO: Zero registers and RAM
  memset(cpu->regist, 0, sizeof(cpu->regist));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
