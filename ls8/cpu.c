#include "cpu.h"
#include "stdio.h"
#include "string.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void stack_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = value;
}

unsigned char stack_pop(struct cpu *cpu)
{
  unsigned char value = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;
  return value;
}
void cpu_load(char *filename, struct cpu *cpu)
{

  FILE *fp;
  char line[1024];
  int address = 0;
  char *pointer;

  fp = fopen(filename, "r");
  while (fgets(line, sizeof line, fp) != NULL)
  {
    unsigned char byte = strtoul(line, &pointer, 2);
    ;
    if (pointer == line)
    {
      continue;
    }
    cpu->ram[address++] = byte;
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] *= cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] += cpu->registers[regB];
    printf("%d add\n", cpu->PC);
    break;
  case ALU_CMP:
  //printf("%d! %d!!\n", cpu->registers[regA], cpu->registers[regB]);
      if (cpu->registers[regA] > cpu->registers[regB]) {
        cpu->FL[6] = 1;
        cpu->FL[7] = 0;
        cpu->FL[5] = 0;
      }
      else if (cpu->registers[regA] < cpu->registers[regB]) {
        cpu->FL[5] = 1;
        cpu->FL[6] = 0;
        cpu->FL[7] = 0;
      }
      else if (cpu->registers[regA] == cpu->registers[regB]) {
        cpu->FL[7] = 1;
        cpu->FL[5] = 0;
        cpu->FL[6] = 0;
      }
      break;
  

    // TODO: implement more ALU ops
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
                   /*   unsigned char *PC = cpu->PC;
  unsigned char *reg = cpu->registers;
  unsigned char *ram = cpu->ram; */
  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    int IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case HLT:
      running = 0;
      break;

    case LDI:
      cpu->registers[operandA] = operandB;
      //printf("%d~\n", cpu->registers[operandA]);
      cpu->PC = cpu->PC + 2;
      break;

    case PRN:
      printf("%d\n", cpu->registers[operandA]);
      cpu->PC = cpu->PC + 1;
      break;

    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      cpu->PC = cpu->PC + 2;
      break;

    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      cpu->PC = cpu->PC + 2;
      break;
    case PUSH:
      stack_push(cpu, operandA);
      cpu->PC = cpu->PC + 1;
      break;

    case POP:
      stack_pop(cpu);
      cpu->PC = cpu->PC + 1;
      break;

    case RET:
      cpu->PC = stack_pop(cpu);
      printf("%d ret\n", cpu->PC);
      break;

    case CALL:
      stack_push(cpu, cpu->PC + 2);
      cpu->PC = cpu->registers[operandA];
      printf("%d call\n", cpu->PC);
      break;

    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);
      cpu->PC = cpu->PC + 2;
      break;
    case JEQ:
      if (cpu->FL[7] == 1) {
        cpu->PC = cpu->registers[operandA];
      }
      else {
        cpu->PC = cpu->PC + 1;
      }
      break;
    case JNE:
      if (cpu->FL[7] != 1) {
        cpu->PC = cpu->registers[operandA];
      }
      else {
        cpu->PC = cpu->PC + 1;
      }
      break;
    case JMP:
      cpu->PC = cpu->registers[operandA];
      break;
    }

    // 4. Move the PC to the next instruction.
    //printf("%d!! %d??\n", IR, cpu->PC);
    cpu->PC += 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->registers[SP] = EMPTY_STACK;
  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->FL, 0, sizeof cpu->FL);
}
