#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int item){
  cpu->ram[++cpu->PC] = item;
  return;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  
  FILE *fp;
  char line[8192];
  int address = 0;
  printf("file name >>> %s", filename);
  fp = fopen(filename, "r");
  if(fp == NULL){
    printf("Wrong!!!");
  } else {
    printf("Right!!!");
  }
  while(fgets(line, sizeof(line), fp) != NULL){
    printf(">>>>>>> %d\n", strtoul(line, NULL, 10));
    // cpu->ram[address] = strtoul(line, NULL, 10);
    // printf("RAM ITEM >>> %d\n", cpu->ram[address]);
    // address++;
  }

  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };
  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }
  // TODO: Replace this with something less hard-coded
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
  int running = 1; // True until we get a HLT instruction

  while (running) {
    unsigned char current_instruction = cpu_ram_read(cpu, cpu->PC);
    unsigned char operand1 = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operand2 = cpu_ram_read(cpu, cpu->PC+2);

    switch(current_instruction){
      case LDI:
        cpu->registers[operand1] = operand2;
        cpu->PC = cpu->PC+3;
        break;
      case PRN:
        printf("Printing %d\n", cpu->registers[0]);
        cpu->PC = cpu->PC+2;
        break;
      case HLT:
        printf("LS-8 stopping.  Goodbye!\n");
        break;
      default:
        exit(1);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}