#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define DATA_LEN 6
#define DEBUG 1
#define IM 5 
#define IS 6
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  int address = 0;
  char line[32];
  char *ptr;
  while (fgets(line, sizeof line, fp) != NULL) {
    cpu->ram[address++] = strtoul(line, &ptr, 2);
    #if DEBUG
      printf("Line that is not used: %s\n", ptr);
      #endif
  }
  fclose(fp);
  #if DEBUG
    for (int i = 0; i < 64; i++) {
      printf("RAM at index %d: %d\n", i, cpu->ram[i]);
    }
    #endif
  

}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;
    case ALU_ADD:
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;
    default:
      printf("Default case reached. No ALU function triggered.\n");

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char item)
{
  cpu->ram[index] = item;
}
void handleInterrupt(struct cpu *cpu) {
  // TODO:
  // disable interrupts
  cpu->registers[IS] = 0;
  // pushing onto the stack
  cpu->ram[--cpu->registers[SP]] = cpu->PC;
  cpu->ram[--cpu->registers[SP]] = cpu->FL;
  cpu->ram[--cpu->registers[SP]] = cpu->registers[0];
  cpu->ram[--cpu->registers[SP]] = cpu->registers[1];
  cpu->ram[--cpu->registers[SP]] = cpu->registers[2];
  cpu->ram[--cpu->registers[SP]] = cpu->registers[3];
  cpu->ram[--cpu->registers[SP]] = cpu->registers[4];
  cpu->ram[--cpu->registers[SP]] = cpu->registers[5];
  cpu->ram[--cpu->registers[SP]] = cpu->registers[6];
  if (cpu->registers[IS] == 1) {
    cpu->PC = cpu->ram[0xF8];
  } else if (cpu->registers[IS] = 2) {
   cpu->PC = cpu->ram[0xF9];
  }
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // timer implementation
    struct timeval tv;
    // gettimeodday() get's the time of day for the computer's clock
    gettimeofday(&tv, NULL);
    // for double time_in_mill and time_diff , initialization
    double time_in_mill;
    double time_diff;
    // in the if statement below you are converting the time in seconds to  milliseconds.
    if (time_in_mill) {
     time_diff = ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000) - time_in_mill;
    }
    time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond
    // these print statements are printing the time in milliseconds and the difference in time from the last loop
    #if DEBUG
    printf("time: %f\n", time_in_mill);
    printf("difference in time: %f\n", time_diff);
    #endif
    int interrupt_happened;
    if (!interrupt_happened) {
    unsigned char maskedInterrupts = cpu->registers[IM] & cpu->registers[IS];
      for (int i = 0; i < 8; i++) {
      if (((maskedInterrupts >> i )&1 )== 1) {
      interrupt_happened = 1;
        handleInterrupt(cpu);
        break;
        }
      }
    }
    int PC = cpu->PC;
    unsigned char IR = cpu_ram_read(cpu, PC);
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);
    if (time_diff >= 1000) {
      IR = 0b01010010;
      cpu->registers[0] = 1;
      operandA = 0;
    }
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case HLT:
        #if DEBUG
          printf("HLT\n");
        #endif
        running = 0;
        break;
      case LDI:
        #if DEBUG
          printf("LDI\n");
        #endif
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        #if DEBUG
          printf("RET\n");
        #endif
        printf("%d\n", cpu->registers[operandA]);
        break;
      case MUL:
        #if DEBUG
          printf("MUL\n");
        #endif
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case PUSH:
        #if DEBUG
          printf("PUSH\n");
        #endif
        cpu->ram[--cpu->registers[SP]] = cpu->registers[operandA];
        break;
      case POP:
        #if DEBUG
          printf("POP\n");
        #endif
        cpu->registers[operandA] = cpu->ram[cpu->registers[SP]++];
        break;
      case CALL:
        #if DEBUG
          printf("CALL\n");
        #endif
        cpu->ram[--cpu->registers[SP]] = PC+2;
        cpu->PC = cpu->registers[operandA] + 1;
        #if DEBUG
          printf("What we're setting PC to in CALL: %d\n", cpu->registers[operandA]);
        #endif
        break;
      case RET:
        #if DEBUG
          printf("RET\n");
        #endif
        cpu->PC = cpu->ram[cpu->registers[SP]++];
        break;
      case ADD:
        #if DEBUG 
          printf("ADD\n");
        #endif
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      case ST:
        #if DEBUG 
          printf("ST\n");
        #endif
        cpu->ram[cpu->registers[operandA]] = cpu->registers[operandB];
        break;
      case INT:
        #if DEBUG
          printf("INT\n");
        #endif
        cpu->registers[IS] = cpu->registers[operandA];
        break;
      case IRET:
        #if DEBUG
          printf("IRET\n");
        #endif
        cpu->registers[6] = cpu->ram[cpu->registers[SP]++];
        cpu->registers[5] = cpu->ram[cpu->registers[SP]++];
        cpu->registers[4] = cpu->ram[cpu->registers[SP]++];
        cpu->registers[3] = cpu->ram[cpu->registers[SP]++];
        cpu->registers[2] = cpu->ram[cpu->registers[SP]++];
        cpu->registers[1] = cpu->ram[cpu->registers[SP]++];
        cpu->registers[0] = cpu->ram[cpu->registers[SP]++];
        cpu->FL = cpu->ram[cpu->registers[SP]++];
        cpu->PC = cpu->ram[cpu->registers[SP]++];
        interrupt_happened = 0;
        break;
      case JMP:
        #if DEBUG
          printf("JMP\n");
        #endif
        cpu->PC = cpu->ram[cpu->registers[operandA]];
        break;
      case PRA:
        #if DEBUG
          printf("PRA\n");
        #endif
        printf("%c\n", cpu->registers[operandA]);
        break;
      case LD:
        #if DEBUG
          printf("LD\n");
        #endif
        cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[operandB]);
        break;
      default:
        printf("Instruction number: %d\n", IR);
        printf("Default case reached. Command invalid.\n");
        break;
    }
    // 4. Move the PC to the next instruction.
    if (!((IR >> 4) & 1)) {
      cpu->PC += 1 + (IR >> 6);
    }
    #if DEBUG
      // sleep(1);
      #endif
    
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, 7);
  cpu->registers[SP] = 0xF4;
  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram, 0, 256);
}