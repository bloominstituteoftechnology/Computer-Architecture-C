#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SP 7

void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->reg[SP]--;
  cpu->ram[cpu->reg[SP]] = val;
  // cpu_ram_write(cpu, val, cpu->reg[SP]);
}

unsigned char cpu_pop(struct cpu *cpu)
{
  // unsigned char ret = cpu_ram_read(cpu, cpu->reg[SP]);
  unsigned char ret = cpu->ram[cpu->reg[SP]];
  cpu->reg[SP]++;
  return ret;
}

// Write the given value to the LS8's Ram at the given address
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

// Read the value at address and return it
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char addr = 0x00; // initialize RAM address to start writing to

  // open the source file
  if (fp == NULL) {
    fprintf(stderr, "Trouble opening the file.\n");
    exit(2);
  }

  // read the source file line by line
  while (fgets(line, sizeof(line), fp) != NULL) { // char *fgets(char *str, int n, FILE *stream)
    // convert these binary strings to numbers
    char *endptr;
    unsigned char ret = strtoul(line, &endptr, 2); // stored as numerical value, but prints to binary or hexvalues

    if (endptr == line) { // ALT1 ignore lines from which no numbers were read, endptr gets set to line if no numbers converted
      continue;
    }

    // write ret to RAM
    // increment RAM address by how much was written to it
    cpu_ram_write(cpu, addr++, ret); // void cpu_ram_write(struct cpu *cpu, unsigned char value, unsigned char address)
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // TODO
  switch (op) {
    case ALU_MUL:
      cpu->reg[regA] *= cpu->reg[regB];
      break;
    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
      break;
    default:
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
  unsigned char IR, operandA, operandB;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu->PC);
    int num_operands = IR >> 6;

    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    if (num_operands == 2) {
      operandA = cpu_ram_read(cpu, cpu->PC+1 & 0xff); // 0xff is for "masking" to avoid implicit sign extension
      operandB = cpu_ram_read(cpu, cpu->PC+2 & 0xff);
    }
    else if (num_operands == 1) {
      operandA = cpu_ram_read(cpu, cpu->PC+1 & 0xff);
    }

    // TRACER
    // printf("TRACE: %02X: %02X %02X %02X\n", cpu->PC, IR, operandA, operandB);
    // printf("TRACE: %2s: %02X %02X %02X\n", cpu->ram, IR, operandA, operandB);

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(IR)
    {
      case LDI:
        cpu->reg[operandA] = operandB; // operandA here is the register, operandB is the value (see LS8-spec)
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case HLT:
        running = 0;
        break;
      
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB); // alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
        break;

      // ### ADD
      // *This is an instruction handled by the ALU.*
      // `ADD registerA registerB`
      // Add the value in two registers and store the result in registerA.  
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB); // alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
        break;

      case PUSH:
        cpu_push(cpu, cpu->reg[operandA]); // void cpu_push(struct cpu *cpu, unsigned char val)
        // cpu->reg[SP]--;
        // cpu->ram[cpu->reg[SP]] = cpu->reg[operandA]; // store the value at operandA into ram
        break;

      case POP:
        cpu->reg[operandA] = cpu_pop(cpu); // unsigned char cpu_pop(struct cpu *cpu)
        // cpu->reg[operandA] = cpu->ram[cpu->reg[SP]]; // pull the value at operandA into ram
        // cpu->reg[SP]++;
        break;

      // ### CALL register
      // Calls a subroutine (function) at the address stored in the register.
      // 1. The address of the ***instruction*** _directly after_ `CALL` is
      //   pushed onto the stack. This allows us to return to where we left off when the subroutine finishes executing.
      // 2. The PC is set to the address stored in the given register. We jump to that location in RAM and execute the first instruction in the subroutine. The PC can move forward or backwards from its current location.
      case CALL:
        cpu->reg[SP]--;
        // cpu->ram[cpu->reg[SP]] = cpu->PC+1;
        cpu_push(cpu, cpu->PC+1);
        cpu->PC = cpu->reg[operandA]-2;
        break;

      // ### RET
      // Return from subroutine.
      // Pop the value from the top of the stack and store it in the `PC`.
      case RET:
        cpu->PC = cpu_pop(cpu);
        // cpu->PC = cpu->ram[cpu->reg[SP]];
        // cpu->reg[SP]++;
        break;

      default:
        break;
    }

    // 6. Move the PC to the next instruction.
    cpu->PC += num_operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */

void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0x00; // when programs start getting loaded (ADDR_PROGRAM_ENTRY)
  cpu->reg[SP] = 0xF4; // where SP is on the empty stack (ADDR_EMPTY_STACK)
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
