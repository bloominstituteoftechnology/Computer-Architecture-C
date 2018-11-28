#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/* stack pointer (SP) needs to be 
initialized with some value and set 
as point to a memory location
*/

// *** push function for cpu stack
// parameters: cpu struct and input value
void cpu_push(struct cpu *cpu, unsigned char value)
{
  // decrement the SP
  cpu->registers[SP]--;
  // set the value in the given register to the address indicated by SP
  cpu->ram[cpu->registers[SP]] = value;
}

// *** pop function for cpu stack
// parameter: is the cpu struct
unsigned char cpu_pop(struct cpu *cpu)
{
// copy the value from the address pointed to by SP to the given register
  unsigned char value = cpu->ram[cpu->registers[SP]];
  // increment the SP
  cpu->registers[SP]++;
  // return the value popped
  return value;
}

// Helper functions cpu_ram_read() and cpu_ram_write()
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar){
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr){
  cpu->ram[mar] = mdr;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   // need to parse an external file
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // // TODO: Replace this with something less hard-coded

  FILE *fp = fopen(filename, "r");
  char line[255];
  unsigned char address = 0x00;

  if(fp == NULL) {
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2); //comes from stdlib
  }

  while(fgets(line, sizeof line, fp) != NULL){
    unsigned char b = strtol(line, NULL, 2);
    cpu_ram_write(cpu,address++, b);
  }
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
    case ALU_ADD:
      cpu->registers[regA] += cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  printf("\nCPU running\n");

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu->ram[cpu->PC];
    unsigned char operandA = cpu->ram[(cpu->PC + 1) & 0xFF];
    unsigned char operandB = cpu->ram[(cpu->PC + 2) & 0xFF];

    // comment this out for print 8 - this will print all registers
    for (int i = 0; i < 8; i++) {
      printf("%d\n", cpu->registers[i]);
    }

    // 2. switch() over it to decide on a course of action.
    switch(IR) {
      // 3. Do whatever the instruction should do according to the spec.
      case LDI:
      printf(" ....... LDI ...... \n");
      cpu->registers[operandA] = operandB;
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      break;

      case PRN:
      printf(" ....... PRN ...... \n");
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      printf("%d\n", cpu->registers[operandA]);
      break;

      case ADD:
      printf(" ....... ADD ...... \n");
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      alu(cpu, ALU_ADD, operandA, operandB);
      break;

      case MUL:
      printf(" ....... MUL ...... \n");
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      alu(cpu, ALU_MUL, operandA, operandB);
      break;

      case HLT:
      printf(" ....... HLT ...... \n");
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      running = 0;
      break;

      case PUSH:
      printf(" ...... PUSH ...... \n");
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      cpu_push(cpu, cpu->registers[operandA]);

      case POP:
      printf(" ...... POP ...... \n");
      printf("RAM at F4: %d\n", cpu->ram[(cpu->PC + 4) & 0xF4]);
      printf("RAM at F3: %d\n", cpu->ram[(cpu->PC + 3) & 0xF3]);
      printf("RAM at F2: %d\n", cpu->ram[(cpu->PC + 2) & 0xF2]);
      cpu->registers[operandA] = cpu_pop(cpu);
    
      // RET
      // CALL

    }
    // 4. Move the PC to the next instruction.
    cpu->PC += (IR >> 6) + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  printf("\nCPU init\n");
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->registers, 0, sizeof(cpu->registers));

  // Initialize stack pointer
  cpu->registers[SP] = 0xF4;
}

