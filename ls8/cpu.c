#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include "cpu.h"

#define DATA_LEN 6

// unsigned char RAM[256];
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR) {
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR) {
  cpu->ram[MAR] = MDR;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // open the file
  FILE *fp = fopen(filename, "r");
  
  if (fp == NULL) {
    fprintf(stderr, "ls8: error opening file %s\n", filename);
    exit(2);
  }
  
  char line[8192];   // will break if whitespace or comments > 8192
  int address = 0;

  while(fgets(line, sizeof line, fp) != NULL) {
    char *endptr;     // ignores comments (prefaced with #) and whitespace.
    unsigned char val = strtoul(line, &endptr, 2);

    if (endptr == line) {
      continue;
    }

    cpu_ram_write(cpu, address++, val);
  }

  // TODO: Replace this with something less hard-coded
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000, // R0
  //   0b00001000, // 8
  //   0b01000111, // PRN R0
  //   0b00000000, // R0
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // (void)cpu;  // define type later
  // (void)regA; // define type later
  // (void)regB; // define type later

  unsigned char valA = cpu->reg[regA];
  unsigned char valB = cpu->reg[regB];

  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] = valA * valB;
      break;

    case ALU_ADD:
      cpu->reg[regA] = valA + valB;
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 * `PC`: Program Counter, address of the currently executing instruction
* `IR`: Instruction Register, contains a copy of the currently executing instruction
* `MAR`: Memory Address Register, holds the memory address we're reading or writing
* `MDR`: Memory Data Register, holds the value to write or the value just read
* `FL`: Flags, see below
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
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operand0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operand1 = cpu_ram_read(cpu, cpu->PC + 2);

    printf("TRACE: %02X: %02X   %02X %02X\n", cpu->PC, IR, operand0, operand1);

    switch(IR) {
      case LDI:
        cpu->reg[operand0] = operand1;
        cpu->PC +=3;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operand0]);
        cpu->PC += 2;
        break;
      case ADD:
        alu(cpu, ALU_ADD, operand0, operand1);
        cpu->PC +=3;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operand0, operand1);
        cpu->PC +=3;
        break;
      case HLT:
        running = 0;
        break;
      default:
        printf("unexpected instruction 0x%02x 0x%02x\n", IR, cpu->PC);
        exit(1);
    }
  }
}
/* It needs to read the memory address that's stored in register `PC`, and store that result in `IR`, the _Instruction Register_. This can just be a local variable in `cpu_run()`.

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
  cpu->PC = 0;
  cpu->FL = 0;

  for (int i = 0; i < 7; i++) {
    cpu->reg[i] = 0;
  }

  cpu->reg[7] = 0xF4;

  memset(cpu->ram, 0, sizeof(cpu->ram));
}
