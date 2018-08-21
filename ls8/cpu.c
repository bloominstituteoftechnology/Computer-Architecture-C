#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *my_cpu)
{
  char data[6] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };


  // int address = 0;

  for (int i = 0; i < 6; i++) {
    my_cpu->ram[i] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}


// Make cpu_ram_read and cpu_ram_write
// they do what they sound like

unsigned char cpu_ram_read(struct cpu *my_cpu, int index) {
  return my_cpu->ram[index];
}

void cpu_ram_write(struct cpu *my_cpu, int index, unsigned char new_symbol) {
  my_cpu->ram[index] = new_symbol;
}

/**
 * ALU
 */
void alu(struct cpu *my_cpu, enum alu_op op, unsigned char regA, unsigned char regB)
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
void cpu_run(struct cpu *my_cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {


    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(my_cpu, my_cpu->PC);
    // 2. switch() over it to decide on a course of action.

    unsigned char diff = ((IR >> 6) & 0b11) + 1;

    int arg_slot_1 = cpu_ram_read(my_cpu, my_cpu->PC + 1);
    unsigned char arg_slot_2 = cpu_ram_read(my_cpu, my_cpu->PC + 2);
    switch(IR) {
      case LDI:
        my_cpu->registers[arg_slot_1] = arg_slot_2;
        break;
      case PRN:
        printf("%d\n", my_cpu->registers[arg_slot_1]);
        break;
      case HLT:
        running = 0;
        break;
    }
    my_cpu->PC += diff;
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *my_cpu)
{
  // TODO: Initialize the PC and other special registers
  // TODO: Zero registers and RAM
  my_cpu->PC = 0;
  my_cpu->registers = (unsigned char *)calloc(8, sizeof(unsigned char));
  my_cpu->ram = (unsigned char *)calloc(256, sizeof(unsigned char));
}
