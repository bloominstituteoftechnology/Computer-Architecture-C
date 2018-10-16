#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define DATA_LEN 6

// read from memory mar = memory address register
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

// Write to memory
void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  printf("CPU Loading....\n");
  unsigned char data[DATA_LEN] = {
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
    // printf("value in address: %d", cpu->ram[address]);
    printf("Data that will be placed into RAM: %hhX\n", data[i]);
    cpu->ram[address++] = data[i];
  }

    printf("Data in RAM0: %x\n", cpu->ram[0]);
    printf("Data in RAM1: %x\n", cpu->ram[1]);
    printf("Data in RAM2: %x\n", cpu->ram[2]);
    printf("Data in RAM3: %x\n", cpu->ram[3]);
    printf("Data in RAM4: %x\n", cpu->ram[4]);
    printf("Data in RAM5: %x\n", cpu->ram[5]);
    printf("Data in RAM6: %x\n", cpu->ram[6]);

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

  unsigned char IR; //instruction register
 
  printf("CPU Running....\n");
  while (running) {
    // 1. Get the value of the current instruction (in address PC).

    IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xFF);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xFF);


    printf("\nTrace:\n");
    printf("PC: %02X\n" , cpu->PC);
    printf("Instruct Reg: %02X\n", IR); 
    printf("Operand A: %02X\n", operandA);
    printf("Operand B: %02X\n", operandB);


    int add_to_pc = (IR >> 6) + 1;
      // 2. switch() over it to decide on a course of action.
    switch (IR) {
      case LDI:
        // LDI
        // 3. Do whatever the instruction should do according to the spec.
        printf("Command is LDI...\n");
        cpu->reg[operandA] = operandB;
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
      
      case HLT:
        running = 0;
        break;
    }

    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
  }


}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Power On State
  // R0-R6 are cleared to 0.
  // R7 is set to 0xF4.
 
  // PC and FL registers are cleared to 0.
  // RAM is cleared to 0.
  printf("CPU Init starting...\n");
  
  // Initialize the PC and other special registers
  cpu->PC = 0;

  // // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));

 cpu->reg[7] = 0b11110100;
}


