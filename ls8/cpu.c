#include "cpu.h"
#include "stdio.h"
#include "string.h"

#define DATA_LEN 6

// Write the given value to the LS8's RAM at the given address
void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR) {
  cpu->ram[MAR] = MDR;
}

// Read the value at address and return it
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR) {
  return cpu->ram[MAR];
}

// Pop item from the stack
void pop(struct cpu *cpu, unsigned char operand) {
  cpu->reg[operand] = cpu_ram_read(cpu, cpu->reg[SP]);
  cpu->reg[SP]++;
}

// Push item to the stack
void push(struct cpu *cpu, unsigned char operand) {
  cpu->reg[SP]--;
  cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operand]);
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, unsigned char *argv)
{
  FILE *fp;
  char line[1024];

  // Initialize RAM address to start writing to
  int address = 0;

  // Open the source file
  if ((fp = fopen(argv, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", argv);
    exit(1);
  }

  // Read in the source file line by line
  while(fgets(line, sizeof(line), fp) != NULL) {

    // convert these to binary strings to numbers
    char *ptr;
    unsigned char ret = strtol(line, &ptr, 2);

    // Ignore lines from which no numbers were read
    if (ptr == line) {
      continue;
    }

    // Write to RAM
    // Increment RAM address by how much was written to it
    cpu->ram[address++] = ret;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;

  switch (op) {
    case ALU_MUL:
      reg[regA] *= reg[regB];
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
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    // 2. Figure out how many operands this next instruction requires

    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    unsigned char move_pc = (IR >> 6) + 1;

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case HLT:
        // Break the loop
        running = 0;
        break;
      case LDI:
        // set the current operation to the next operation
        cpu->reg[operandA] = operandB;
        break;
      case PRN:
        // Print the value at the given register
        printf("%d\n", cpu->reg[operandA]);
        break;
      case MUL:
        // Multiple two numbers together
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      case POP:
        // Pop item from stack
        pop(cpu, operandA);
        break;
      case PUSH:
        // Push item to stack
        push(cpu, operandA);
        break;
      case CALL:
        PushPop item from stack
        cpu->reg[SP]--;
        cpu_ram_write(cpu, cpu->reg[SP], cpu->PC + move_pc);

        // Set PC to the operand
        cpu->PC = cpu->reg[operandA];
        cpu->add_pc = 1;
        break;
      case RET:
        // Pop item from stack
        cpu->PC = cpu_ram_read(cpu, cpu->reg[SP]);
        cpu->reg[SP]++;

        // Set PC to next instruction
        cpu->add_pc = 1;
        break;

    }

    // 6. Move the PC to the next instruction.
    if (cpu->add_pc == 0) {
      cpu->PC += move_pc;
    } else {
      cpu->add_pc = 0;
      continue;
    }

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = ADDR_PROGRAM_ENTRY;
  cpu->reg[SP] = ADDR_EMPTY_STACK;
  cpu->add_pc = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
