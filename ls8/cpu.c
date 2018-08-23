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
    printf("%d\n", cpu->reg[cpu->ram[cpu->PC + 1]]);
    printf("R%d, set to %d * %d = %d\n", cpu->ram[cpu->PC + 1], regA, regB, cpu->reg[cpu->ram[cpu->PC + 1]]);
    break;
  // TODO: implement more ALU ops
  case ALU_ADD:
    printf("ALU_ADD: HANDLER FOUND\n");
    printf("regA = %d and regB = %d\n", regA, regB);
    cpu->reg[cpu->ram[cpu->PC + 1]] = regA + regB;
    printf("Reg-index %d, set to %d + %d = %d\n", cpu->ram[cpu->PC + 1], regA, regB, cpu->reg[cpu->ram[cpu->PC + 1]]);
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
    printf(">>>>>  IR is a PC_mutator? %d\n", (cpu->IR >> 4) & 1);

    if (((cpu->IR >> 4) & 1) == 0) // Chekc is the instruction is a PC_Mutator, if not jump to next instruction.
    {
      // printf("IR = %d\n", cpu->IR);
      instruction_operands = (cpu->IR >> 6);
      // printf("IR >> 6 = %d\n", cpu->IR);
      cpu->PC = PC + instruction_operands + 1;
    }
    // printf("cpu->PC next instruction index: %d\n", cpu->PC);
    PC = cpu->PC;
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
  cpu->reg[SP] = 0xF3;
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
    alu(cpu, ALU_ADD, cpu->reg[cpu->ram[cpu->PC + 1]], cpu->reg[cpu->ram[cpu->PC + 2]]);
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
  case CALL:
    printf("CALL. HANDLER FOUND\n");
    cpu->reg[SP] -= 1;
    printf("SP move from %d to %d\n", cpu->reg[SP] + 1, cpu->reg[SP]);
    cpu->ram[cpu->reg[SP]] = cpu->PC + (cpu->IR >> 6) + 1;
    printf("CALL. Set RAM[SP] = %d\n", cpu->ram[cpu->reg[SP]]);
    cpu->PC = cpu->reg[cpu->ram[cpu->PC + 1]];
    printf("CALL. PC set to %d\n", cpu->PC);
    break;
  case RET:
    printf("RET. HANDLER FOUND\n");
    cpu->PC = cpu->ram[cpu->reg[SP]];
    printf("Set PC to %d", cpu->PC);
    cpu->reg[SP] += 1;
    printf("SP move from %d to %d\n", cpu->reg[SP] - 1, cpu->reg[SP]);
    break;
  // case INT:
  // case IRET:
  case JMP:
    printf("JMP. HANDLER FOUND\n");
    cpu->PC = cpu->reg[cpu->ram[cpu->PC + 1]];
    printf("PC set to %d\n", cpu->PC);
    break;
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
  case ST:
    printf("ST. HANDLER FOUND\n");
    cpu->reg[cpu->ram[cpu->PC + 1]] = cpu->reg[cpu->ram[cpu->PC + 2]];
    printf("Set reg[%d] = (reg[%d] value: %d)", cpu->PC + 1, cpu->PC + 2, cpu->reg[cpu->ram[cpu->PC + 1]]);
    break;
  case PUSH:
    printf("PUSH. HANDLER FOUND\n");
    cpu->reg[SP] -= 1;
    printf("SP move form %d to %d\n", cpu->reg[SP] + 1, cpu->reg[SP]);
    cpu->ram[cpu->reg[SP]] = cpu->reg[cpu->ram[cpu->PC + 1]];
    printf("Copied value %d form REG[%d] to RAM[%d]\n", cpu->ram[cpu->reg[SP]], cpu->ram[cpu->PC + 1], cpu->reg[SP]);
    break;
  case POP:
    printf("POP. HANDLER FOUND\n");
    cpu->reg[cpu->ram[cpu->PC + 1]] = cpu->ram[cpu->reg[SP]];
    printf("Copied value %d form RAM[%d] to REG[%d]\n", cpu->ram[cpu->reg[SP]], cpu->reg[SP], cpu->ram[cpu->PC + 1]);
    cpu->reg[SP] += 1;
    printf("SP move form %d to %d\n", cpu->reg[SP], cpu->reg[SP] - 1);
    break;
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