#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
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
    cpu->ram[address++] = data[i];
  }

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
 * Reads memory data (MDR) from memory address (MAR) - cpu_run() helper function
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

/**
 * Writes memory data (MDR) to memory address (MAR) - cpu_run() helper function
 */
void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  cpu->ram[MAR] = MDR;
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
    unsigned char IR = cpu_ram_read(cpu, cpu->PC); // IR store results from memory address that's stored in register PC

    // 2. Figure out how many operands this next instruction requires
      //Some instructions requires up to the next two bytes of data _after_ the `PC` in memory to perform operations on.
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned operandA = cpu_ram_read(cpu, (cpu->PC + 1));
    unsigned operandB = cpu_ram_read(cpu, (cpu->PC + 2));

    printf("TRACE: %02X: %02X   %02X %02X\n", cpu->PC, IR, operandA, operandB);

    // 4. switch() over it to decide on a course of action.
    switch(IR)
    {
      // 5. Do whatever the instruction should do according to the spec.
      // 6. Move the PC to the next instruction.
      case LDI: //load "immediate", store a value in a register, or "set this register to this value".
      cpu->reg[operandA] = operandB; //Loads registerA with the value at the memory address stored in registerB.
      cpu->PC += 3; //moves PC to PRN
      break;

      case PRN: //a pseudo-instruction that prints the numeric value stored in a register.
      printf("%d\n", cpu->reg[operandA]); 
      cpu->PC += 2; //moves PC to HLT
      break;

      case HLT: //halt the CPU and exit the emulator.
        exit(0);
        break;

      default:
        printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->PC);
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
  memset(cpu->ram, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
