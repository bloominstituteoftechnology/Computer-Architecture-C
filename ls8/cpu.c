#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *arg)
{
  if (argc < 2){
    printf("please provide additional argument\n");
  } else {
    FILE *fp;
    fp = fopen(arg, "r");
    char instruction[9];
    for (int i=0; i<256; i++){
      if (fgets(instruction, 9, fp) != NULL) {
        cpu->ram[i] = strtoul(instruction, NULL, 2);
        printf("wats in tha ram: %d\n", cpu->ram[i]);
        while(fgetc(fp) != '\n');
      } else {
        break;
      }
    }
    fclose(fp);
  }

  // int address = 0;

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
  // switch (op) {
  //   case ALU_MUL:
  //     // TODO
  //     break;

  //   // TODO: implement more ALU ops
  // }
}

char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, int item)
{
  cpu->ram[index] = item;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    int c = cpu->PC;
    unsigned char instruction = cpu->ram[c];
    switch(instruction) {
      case LDI:
        cpu->registers[(cpu->ram[c+1])] = cpu->ram[c+2];
        printf("ram write: %d\n", cpu->registers[0]);
        cpu->PC += 3;
        break;
      case PRN:
        printf("print: %d\n", cpu->registers[(cpu->ram[c+1])]);
        cpu->PC += 2;
        break;
      case HLT:
        running = 0;
        printf("fin\n");
        break;
      default:
        printf("could not read instruction\n");
        running = 0;
        break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->registers[0], 8, 0);
  memset(cpu->ram[0], 256, 0);
}
