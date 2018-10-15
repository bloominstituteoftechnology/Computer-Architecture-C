#include "cpu.h"

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
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int IR;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    char val = cpu->pc;
    IR = val;
    // 2. switch() over it to decide on a course of action.
    int operandA = cpu_ram_read(cpu.ram['PC+1']);
    int operandB = cpu_ram_read(cpu).ram['PC+2'];

    switch(){
      case:

      default:
    }
    //Some instructions requires up to the next two bytes of data _after_ the `PC` in
//memory to perform operations on. Sometimes the byte value is a register number,
//other times it's a constant value (in the case of `LDI`). Using
//`cpu_ram_read()`, read the bytes at `PC+1` and `PC+2` from RAM into variables
//`operandA` and `operandB` in case the instruction needs them.

//Then, depending on the value of the opcode, perform the actions needed for the
//instruction per the LS-8 spec. Maybe a `switch` statement...? Plenty of options.
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
  // TODO: Zero registers and RAM
    cpu = malloc(sizeof(cpu));
    cpu->pc = 0;
    cpu->registers[0] = '0';
    cpu->registers[1] = '0';
    cpu->registers[2] = '0';
    cpu->registers[3] = '0';
    cpu->registers[4] = '0';
    cpu->registers[5] = '0';
    cpu->registers[6] = '0';
    cpu->registers[7] = '0xF4';
    cpu->ram = '0x00';
    return cpu;
}

void cpu_ram_read(struct cpu *cpu, int *i)
{
  int fd = open("cpu.h","r");
  fd = read(cpu->ram[i]);
  return fd;
}

void cpu_ram_write(struct cpu *cpu)
{
  int fd = open("cpu.h", "w");
  fprintf("%d\n", cpu->ram);
}