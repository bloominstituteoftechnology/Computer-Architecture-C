#include "cpu.h"

void handle_instruction(struct cpu *cpu);
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  printf("File path %s\n", file);
  const int DATA_LEN = 6;
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op)
//   {
//   case ALU_MUL:
//     // TODO
//     break;

//     // TODO: implement more ALU ops
//   }
// }

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
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

/**
 * HELPERS
*/
void handle_instruction(struct cpu *cpu)
{
  printf("\n\nCURRENT IR: %d\n", cpu->IR);
  switch (cpu->IR)
  {
    // ALU
  case ADD:
  case SUB:
  case MUL:
  case DIV:
  case MOD:
  case INC:
  case DEC:
  case CMP:
  case AND:
  case NOT:
  case OR:
  case XOR:
  case SHL:
  case SHR:
    printf("ALU-instruction. HANDLER FOUND\n");
    // alu(cpu, cpu->IR, cpu->reg[cpu->PC + 1], cpu->reg[cpu->PC + 1]);
    break;
  // PC mutators
  case CALL:
  case RET:
  case INT:
  case IRET:
  case JMP:
  case JEQ:
  case JNE:
  case JGT:
  case JLT:
  case JLE:
  case JGE:
  // Other
  case NOP:
  case HLT:
    printf("HLT. HANDLER FOUND\n");
    cpu->IR = '\0';
    break;
  case LDI:
    printf("LDI. HANDLER FOUND\n");
    cpu->reg[cpu->ram[cpu->PC + 1]] = cpu->ram[cpu->PC + 2];
    printf("LDI. number set to: %d, in R%d\n", cpu->reg[cpu->ram[cpu->PC + 1]], cpu->ram[cpu->PC + 1]);
    break;
  case LD:
  case ST:
  case PUSH:
  case POP:
  case PRN:
    printf("PRN. HANDLER FOUND\n");
    printf("PRN. PRINTED Number %d in R%d\n", cpu->reg[cpu->ram[cpu->PC + 1]], cpu->ram[cpu->PC + 1]);
    break;
  case PRA:
  default:
    printf("The IR %d-instruction has no handler\n", cpu->IR);
  }
};