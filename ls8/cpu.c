#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_instruction(struct cpu *cpu);
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  printf("CPU_LOAD: File path is: '%s'\n", file);
  FILE *file_d = fopen(file, "r");
  if (file_d == NULL)
  {
    fprintf(stderr, "ERROR loading the file %s\n", file);
    exit(1);
  }

  int ram_index = 0;
  // I get reference on how to read lines from a 'FILE pointer' from : https://stackoverflow.com/a/3501681/9355810
  char *line = NULL;
  size_t lenght = 0; // I do not understand what this variable does in 'getline()'
  ssize_t line_length;

  unsigned long binary_code;
  char *end_of_binary_number;

  while ((line_length = getline(&line, &lenght, file_d)) != -1)
  {
    // printf("Retrieved line of length %zu :\n", line_length);
    // printf("File line: %s", line);
    if (line_length > 8)
    {
      binary_code = strtoul(line, &end_of_binary_number, 2);
      // printf("Binary: %lu\n", binary_code);
      if (line != end_of_binary_number)
      {
        // printf("*line == *end_of_binary_number : %d\n\n", line == end_of_binary_number);
        printf("CPU_LOAD: **** Adding entry to RAM index: %d ****\n", ram_index);
        cpu->ram[ram_index++] = binary_code;
      }
    }
  }
  printf("ram_index after reading '%s': %d\n\n", file, ram_index);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  printf("\n\nALU function running\n");
  switch (op)
  {
  case ALU_MUL:
    printf("ALU_MUL: HANDLER FOUND\n");
    printf("regA = %d and regB = %d\n", regA, regB);
    cpu->reg[cpu->ram[cpu->PC + 1]] = regA * regB;
    printf("Reg-index %d, set to %d * %d = %d\n", cpu->ram[cpu->PC + 1], regA, regB, cpu->reg[cpu->ram[cpu->PC + 1]]);
    break;
    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  printf("CPU_RUN running\n");
  int running = 1; // True until we get a HLT instruction

  unsigned char *rgstr = cpu->reg;
  unsigned char *ram = cpu->ram;

  unsigned char PC = cpu->PC;
  unsigned char instruction_operands;
  cpu->IR = ram[PC];

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    // printf("PC %d\nIRd %d\n", PC, cpu->IR);
    // printf("PC %d\nIRc %c\n", PC, cpu->IR);

    handle_instruction(cpu);
    if (cpu->IR == '\0')
    {
      break;
    }
    // printf("IR = %d\n", cpu->IR);
    instruction_operands = cpu->IR >> 6;
    // printf("IR >> 6 = %d\n", cpu->IR);
    PC = cpu->PC = PC + instruction_operands + 1;
    // printf("cpu->PC next instruction index: %d\n", cpu->PC);
    printf("PC next instruction index: %d\n", PC);
    cpu->IR = ram[PC];
  }
  printf("\nCPU_RUN END\n");
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->IR = 0;
  cpu->MAR = 0;
  cpu->MDR = 0;
  cpu->FL = 0;

  // TODO: Zero registers and RAM
  memset(cpu->reg, 0, sizeof(cpu->reg));
  cpu->reg[7] = 0xF3;
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

/**
 * HELPERS
*/
void handle_instruction(struct cpu *cpu)
{
  printf("\n\nCURRENT PC: %d\n", cpu->PC);
  printf("CURRENT IR: %d\n", cpu->IR);
  switch (cpu->IR)
  {
    /* ALU */
  case ADD:
    printf("ALU-ADD. HANDLER FOUND\n");
    // alu(cpu, cpu->IR, cpu->reg[cpu->PC + 1], cpu->reg[cpu->PC + 1]);
    break;
  // case SUB:
  case MUL:
    printf("ALU-MUL. HANDLER FOUND\n");
    alu(cpu, ALU_MUL, cpu->reg[cpu->ram[cpu->PC + 1]], cpu->reg[cpu->ram[cpu->PC + 2]]);
    break;
  // case DIV:
  // case MOD:
  // case INC:
  // case DEC:
  // case CMP:
  // case AND:
  // case NOT:
  // case OR:
  // case XOR:
  // case SHL:
  // case SHR:
  /* PC mutators */
  // case CALL:
  // case RET:
  // case INT:
  // case IRET:
  // case JMP:
  // case JEQ:
  // case JNE:
  // case JGT:
  // case JLT:
  // case JLE:
  // case JGE:
  /* Other */
  // case NOP:
  case HLT:
    printf("HLT. HANDLER FOUND\n");
    cpu->IR = '\0';
    break;
  case LDI:
    printf("LDI. HANDLER FOUND\n");
    cpu->reg[cpu->ram[cpu->PC + 1]] = cpu->ram[cpu->PC + 2];
    printf("LDI. number set to: %d, in R%d\n", cpu->reg[cpu->ram[cpu->PC + 1]], cpu->ram[cpu->PC + 1]);
    break;
  // case LD:
  // case ST:
  case PUSH:
    printf("PUSH. HANDLER FOUND\n");
    cpu->reg[7] -= 1;
    printf("SP move form %d to %d\n", cpu->reg[7] + 1, cpu->reg[7]);
    cpu->ram[cpu->reg[7]] = cpu->reg[cpu->ram[cpu->PC + 1]];
    printf("Copied value %d form REG[%d] to RAM[%d]\n", cpu->ram[cpu->reg[7]], cpu->ram[cpu->PC + 1], cpu->reg[7]);
    break;
  // case POP:
  case PRN:
    printf("PRN. HANDLER FOUND\n");
    printf("%d\n", cpu->reg[cpu->ram[cpu->PC + 1]]);
    printf("PRN. PRINTED Number %d in R%d\n", cpu->reg[cpu->ram[cpu->PC + 1]], cpu->ram[cpu->PC + 1]);
    break;
  case PRA:
  default:
    printf("The IR %d-instruction has no handler\n", cpu->IR);
  }
};