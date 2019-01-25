#include "cpu.h"
#include "stdio.h"
#include "stdlib.h"

#define ADDR_EMPTY_STACK 0xF4 

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Missing arguments. Provide: ./ls8 filename");
    exit(1);
  }

  FILE *fp;
  char line [1024];
  char *file = argv[1];
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  int address = 0;

  if ((fp = fopen(file, "r")) == NULL) {
    fprintf(stderr, "Error: unable to open file %s\n", file);
    exit(1);
  }

  while (fgets(line, sizeof(line), fp) != NULL){

    char *ptr;
    unsigned char instruction = strtol(line, &ptr, 2);

    if (ptr == line) {
      continue;
    }

    cpu->ram[++address] = instruction;
  }

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valB = reg[regB];
  switch (op) {
    case ALU_MUL:
      // TODO
      reg[regA] *= valB;
      break;
    case ALU_ADD:
      // TODO
      reg[regA] += valB;
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu )
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char instruction = cpu->ram[cpu->PC];
    // 2. Figure out how many operands this next instruction requires
    unsigned int num_operands = instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned int operandA;
    unsigned int operandB;
    if (num_operands == 2) {
      operandA = cpu->ram[cpu->PC + 1];
      operandB = cpu->ram[cpu->PC + 2];
    } else if (num_operands == 1) {
      operandA = cpu->ram[cpu->PC + 1];
    }
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (instruction) {
      case HLT:
        running = 0;
        break;

      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        break;

      case LDI:
        cpu->reg[operandA] = operandB;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->SP++);
        break;

      case PUSH:
        cpu_ram_write(cpu, --cpu->SP, cpu->reg[operandA]);
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
  cpu->PC = 0;
  cpu->FL = 0;
  cpu->SP = ADDR_EMPTY_STACK;
  memset(cpu->reg, 0, sizeof(cpu->reg[0]));
  memset(cpu->ram, 0, sizeof(cpu->ram[0]));
}

void cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char val, unsigned char index){
  cpu->ram[index] = val;
}

