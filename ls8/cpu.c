#include <string.h>
#include "cpu.h"

#define DATA_LEN 6

// unsigned char RAM[256];
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {
  cpu->ram[index];
}

unsigned char cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {
  cpu->ram[index] = value;
}

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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}
/*
This is the workhorse function of the entire processor. It's the most difficult
part to write.

It needs to read the memory address that's stored in register `PC`, and store
that result in `IR`, the _Instruction Register_. This can just be a local
variable in `cpu_run()`.

Some instructions requires up to the next two bytes of data _after_ the `PC` in
memory to perform operations on. Sometimes the byte value is a register number,
other times it's a constant value (in the case of `LDI`). Using
`cpu_ram_read()`, read the bytes at `PC+1` and `PC+2` from RAM into variables
`operandA` and `operandB` in case the instruction needs them.

Then, depending on the value of the opcode, perform the actions needed for the
instruction per the LS-8 spec. Maybe a `switch` statement...? Plenty of options.

After the handler returns, the `PC` needs to be updated to point to the next
instruction for the next iteration of the loop in `cpu_run()`. The number of
bytes an instruction uses can be determined from the two high bits (bits 6-7) of
the instruction opcode. See the LS-8 spec for details.
*/

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  memset(cpu->REG, 0, sizeof(cpu->REG));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
