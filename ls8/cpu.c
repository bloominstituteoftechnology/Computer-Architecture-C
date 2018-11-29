#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define DATA_LEN 256

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_name)
{
  int address = 0;
  char line[8192];

  FILE *fp = fopen(file_name,"r");

  while (fgets(line, sizeof line, fp) != NULL) {
    if(line[0] != '#'){
      cpu_ram_write(cpu, address, strtoul(line, NULL, 2));
      address++;
    }
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
      printf("Multiply %d by %d\n", cpu->registers[regA], cpu->registers[regB]);
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;
    case ALU_ADD:
      printf("Add %d and %d\n", cpu->registers[regA], cpu->registers[regB]);
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
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
  unsigned char opA;
  unsigned char opB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char ir = cpu->ram[cpu->PC];
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch (ir) {
      case LDI:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        opB = cpu_ram_read(cpu, cpu->PC+2);
        cpu->registers[opA] = opB;
        break;
      case PRN:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        printf("%d\n", cpu->registers[opA]);
        break;
      case MUL:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        opB = cpu_ram_read(cpu, cpu->PC+2);
        alu(cpu, ALU_MUL, opA, opB);
        break;
      case PUSH:
        cpu->registers[7]--;
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu_ram_write(cpu, cpu->registers[7], cpu->registers[opA]);
        break;
      case POP:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->registers[opA] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        break;
      case CALL:
        cpu->registers[7]--;
        opA = cpu_ram_read(cpu, cpu->PC+1); //subroutine to call
        opB = cpu->PC+2; //address of the next instruction
        cpu_ram_write(cpu, cpu->registers[7], opB); //push next instruction onto stack
        cpu->PC = cpu->registers[opA]; //set PC to subroutine address
        break;
      case RET:
        opA = cpu_ram_read(cpu, cpu->registers[7]); //address of next instruction after CALL
        cpu->registers[7]++;
        cpu->PC = opA;
        break;
      case ADD:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        opB = cpu_ram_read(cpu, cpu->PC+2);
        alu(cpu, ALU_ADD, opA, opB);
        break;
      case CMP:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        opB = cpu_ram_read(cpu, cpu->PC+2);
        if(cpu->registers[opA] == cpu->registers[opB]) {
          cpu->FL = 0b00000001;
        } else if(cpu->registers[opA] < cpu->registers[opB]) {
          cpu->FL = 0b00000100;
        } else {
          cpu->FL = 0b00000010;
        }
        break;
      case JEQ:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->PC += 2;
        if((cpu->FL & 0b00000001) != 0) cpu->PC = cpu->registers[opA];
        break;
      case JNE:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->PC += 2;
        if((cpu->FL & 0b00000001) != 1) cpu->PC = cpu->registers[opA];
        break;
      case JGE:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->PC += 2;
        if((cpu->FL & 0b00000011) != 0) cpu->PC = cpu->registers[opA];
        break;
      case JGT:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->PC += 2;
        if((cpu->FL & 0b00000010) != 0) cpu->PC = cpu->registers[opA];
        break;
      case JLE:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->PC += 2;
        if((cpu->FL & 0b00000101) != 0) cpu->PC = cpu->registers[opA];
        break;
      case JLT:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->PC += 2;
        if((cpu->FL & 0b00000100) != 0) cpu->PC = cpu->registers[opA];
        break;
      case JMP:
        opA = cpu_ram_read(cpu, cpu->PC+1);
        cpu->PC = cpu->registers[opA];
        break;
      case HLT:
        running = 0;
        break;
      default:
        break;
    }

    if((ir&0b00010000) == 0) { //if set PC bit is not set increment PC
      int movePC = (ir >> 6) + 1;
      cpu->PC += movePC;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = malloc(sizeof(struct cpu));
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram,0,256);

  for(int i = 0;i<7;i++){
    cpu->registers[i] = 0;
  }

  cpu->registers[7] = 0xF4;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char loc)
{
  return cpu->ram[loc];
}

void cpu_ram_write(struct cpu *cpu, unsigned char loc, int item)
{
  cpu->ram[loc] = item;
}
