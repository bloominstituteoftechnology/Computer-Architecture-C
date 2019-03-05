#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cpu.h"

#define DATA_LEN 6

unsigned cpu_ram_read(struct cpu *cpu, unsigned char address){
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char val){
  cpu->ram[address] = val;
};

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *path)
{
  FILE *filepointer = fopen(path, "r");
  if(filepointer == NULL){
    printf("Couldn't open the program you entered: %s\n", path);
    exit(2);
  }
  int address = 0;
  char line[8000];
  while(fgets(line, sizeof(line), filepointer) != NULL){
    char *endpointer;
    if(endpointer == line){
      continue;
    }
    
  }
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
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
      // Not needed if we use a switch statement
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char op0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char op1 = cpu_ram_read(cpu, cpu->PC + 2);
    printf("trace: %02X: %02X    %02X   %02X\n", cpu->PC, instruction, op0, op1);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch (instruction){
      case LDI:
        cpu->regs[op0] = op1;
        cpu->PC += 3;
        break;
      case PRN:
        printf("%d\n", cpu->regs[op0]);
        cpu->PC += 2;
        break;
      case HLT: //HLT the cpu and exit the emulator
        running = 0;
        break;
      default:
        printf("unexpected instruction 0x%02X at 0x%02X\n", instruction, cpu->PC);
        exit(1);
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  for(int i = 0; i < 7; i++){
    cpu->regs[i] = 0;
  }
  cpu->regs[7] = 0xF4;

  cpu->PC = 0;
  cpu->FL = 0;
  memset(cpu->ram, 0, sizeof cpu->ram);
}
