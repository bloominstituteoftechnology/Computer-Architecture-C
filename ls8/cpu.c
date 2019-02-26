#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  //open file and assign space for it 
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char ram_address = 0x00; 

  // check for file 
  if (fp == NULL) {
    fprintf(stderr, "can't open file\n");
    exit(2);
  }

  // man page - fgets() reads in at most one less than size characters from stream and stores them into the buffer pointed to by s. 
  // char *fgets(char *str, int n, FILE *stream)
  while (fgets(line, sizeof(line), fp) != NULL) {
    char *end_ptr;
    unsigned char value = strtoul(line, &end_ptr, 2); //convert binary strings

    if (end_ptr == line) {
      // no numbers, so...
      continue;
    }
    // write ret to RAM
    cpu_ram_write(cpu,ram_address++, value);
  }

  fclose(fp);

}


 /* 
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

  // TODO: Replace this with something less hard-coded / step 8
}*/

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

/**
 * ALU
 * `MUL registerA registerB`

Multiply the values in two registers together and store the result in registerA.

Machine code:
```
10100010 00000aaa 00000bbb
A2 0a 0b
```
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO registerA = regA * regb +
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * DAY - 1 / Step 4 
 * Run the CPU
 */

void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction 
  unsigned char IR; //store that result in `IR` ... This can just be a local
  unsigned char operandA;//into variables `operandA` and `operandB` in case the instruction needs them
  unsigned char operandB;
 

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC)./ IR
    IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    operandA = cpu_ram_read(cpu, cpu->PC+1 & 0xff);
    operandB = cpu_ram_read(cpu, cpu->PC+2 & 0xff);

    int next_pc = (IR >> 6) + 1; // >> bitwise opperator, shifts right

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    for (int i = 0; i < 8; i++) {
      printf(" %02X", cpu->reg[i]);
    }

    printf("\n");
    // 4. switch() over it to decide on a course of action. Cases - HTL / PRN / LDI /MUL
    // 5. Do whatever the instruction should do according to the spec.
    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB; 
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;
      
      case MUL: // FILL ME IN 
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      default:
        printf("unexpected instruction 0x%02x at 0x%02x\n", IR, cpu->PC);
        exit(1);
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += next_pc;
  }
}

/**
 * DAY - 1 / Step 3 
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers 
  // At first, the PC, registers, and RAM should be cleared to zero.
  cpu->PC = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  //(`memset()` might help you clear registers and RAM.)
    // man pages -  void *memset(void *s, int c, size_t n);
    // function fills the first n bytes of the memory area pointed to by s with the constant byte c.
}

/*
Meanings of the bits in the first byte of each instruction: `AABCDDDD`

* `AA` Number of operands for this opcode, 0-2
* `B` 1 if this is an ALU operation
* `C` 1 if this instruction sets the PC
* `DDDD` Instruction identifier

The number of operands `AA` is useful to know because the total number of bytes in any
instruction is the number of operands + 1 (for the opcode). This
allows you to know how far to advance the `PC` with each instruction.

operandA = cpu_read_ram(cpu->pc + 1)

*/