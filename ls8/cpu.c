#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

// ================== RAM functions ===================

/**  Read from the CPU's ram at the specifc location 
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
  return cpu->ram[MAR];
}

/**  Writes at index specified for the CPU's ram
 */
void cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  cpu->ram[MAR] = MDR;
}

// ================== Instrcution Handlers ===================

// LDI function: Set the value of a register to an integer
void handle_LDI(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
  cpu->registers[MAR] = MDR;
};

// PRN function: Print numeric value stored in the given register
void handle_PRN(struct cpu *cpu, unsigned char MAR)
{
  printf("%d\n", cpu->registers[MAR]);
}

// HLT function: Halt the CPU (and exit the emulator)
void handle_HLT()
{
  exit(0);
}

// ST function: Store value in registerB in the address stored in registerA
// This opcode writes to memory
void handle_ST(struct cpu *cpu, unsigned char regA, unsigned char regB)
{
  unsigned char MAR = cpu->registers[regA];
  unsigned char MDR = cpu->registers[regB];
  cpu_ram_write(cpu, MAR, MDR);
}

//  JMP function: Jump to the address stored in the given register
void handle_JMP(struct cpu *cpu, unsigned char regA)
{
  unsigned char MAR = cpu->registers[regA];
  cpu->pc = MAR;
}

void handle_PRA(struct cpu *cpu, unsigned char MAR)
{
  printf("%c\n", cpu->registers[MAR]);
}

// JEQ function: If equal flag is set (true), jump to the address stored in the given register
void handle_JEQ(struct cpu *cpu, unsigned char regA, unsigned char nextOpcode)
{
  unsigned char MAR = cpu->registers[regA];

  if ((0b00000001 & cpu->fl))
  {
    cpu->pc = MAR;
  }
  else
  {
    cpu->pc += nextOpcode;
  }
}

// JNE function: If E flag is clear (false, 0), jump to the address stored in the given register.
void handle_JNE(struct cpu *cpu, unsigned char regA, unsigned char nextOpcode)
{
  unsigned char MAR = cpu->registers[regA];

  if (!(0b00000001 & cpu->fl))
  {
    cpu->pc = MAR;
  }
  else
  {
    cpu->pc += nextOpcode;
  }
}

// ================= Stack functions ==================

void handle_PUSH(struct cpu *cpu, unsigned char MDR)
{
  cpu_ram_write(cpu, --cpu->registers[7], MDR);
}

void handle_POP(struct cpu *cpu, unsigned char regA)
{
  unsigned char value = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->registers[regA] = value;
}

// ================= Interrupt Handle functions ==================
void handle_interupt(struct cpu *cpu, int location)
{
  cpu->registers[6] = 0;

  cpu_ram_write(cpu, --cpu->registers[7], cpu->pc);
  cpu_ram_write(cpu, --cpu->registers[7], cpu->fl);
  for (int i = 0; i < 7; i++)
  {
    handle_PUSH(cpu, i);
  }
  cpu->pc = cpu->ram[0xF8 + location];
  cpu->registers[5] = 0;
}

void handle_IRET(struct cpu *cpu)
{
  for (int i = 6; i >= 0; i--)
  {
    handle_POP(cpu, i);
  }
  cpu->fl = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->pc = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->registers[5] = 1;
}

// ================= SubRoutine functions ==================
void handle_CALL(struct cpu *cpu, char unsigned MAR, char unsigned nextPC)
{
  // Saves next PC at the correct spot
  handle_PUSH(cpu, nextPC);
  cpu->pc = cpu->registers[MAR];
}

void handle_RET(struct cpu *cpu)
{
  cpu->pc = cpu_ram_read(cpu, cpu->registers[7]++);
}

void handle_CMP(struct cpu *cpu, unsigned char regA, unsigned char regB)
{
  if (cpu->registers[regA] == cpu->registers[regB])
  {
    cpu->fl = 0b00000001;
  }
  else if (cpu->registers[regA] > cpu->registers[regB])
  {
    cpu->fl = 0b00000010;
  }
  else
  {
    cpu->fl = 0b00000100;
  }
}

// ================== CPU functions ===================

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv)
{
  FILE *ptr = fopen(argv, "r");
  char c[100];
  unsigned char ret;

  int address = 0;
  while (fgets(c, 100, ptr) != NULL)
  {
    char *endptr;
    ret = strtoul(c, &endptr, 2);
    if (endptr == c)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, ret);
  }

  fclose(ptr);

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char regB = cpu_ram_read(cpu, (cpu->pc + 2));
  unsigned char addiVal = cpu_ram_read(cpu, (cpu->pc + 3));
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
    break;
  case ALU_SUB:
    cpu->registers[regA] = cpu->registers[regA] - cpu->registers[regB];
    break;
  case ALU_MOD:
    cpu->registers[regA] = cpu->registers[regA] % cpu->registers[regB];
    break;
  case ALU_DIV:
    cpu->registers[regA] = cpu->registers[regA] / cpu->registers[regB];
    break;
  case ALU_INC:
    cpu->registers[regA]++;
    break;
  case ALU_DEC:
    cpu->registers[regA]--;
    break;
  case ALU_CMP:
    handle_CMP(cpu, regA, regB);
    break;
  case ALU_NOT:
    cpu->registers[regA] = ~cpu->registers[regA];
    break;
  case ALU_AND:
    cpu->registers[regA] = cpu->registers[regA] & cpu->registers[regB];
    break;
  case ALU_OR:
    cpu->registers[regA] = cpu->registers[regA] | cpu->registers[regB];
    break;
  case ALU_XOR:
    cpu->registers[regA] = cpu->registers[regA] ^ cpu->registers[regB];
    break;
  case ALU_SHL:
    cpu->registers[regA] = cpu->registers[regA] >> cpu->registers[regB];
    break;
  case ALU_SHR:
    cpu->registers[regA] = cpu->registers[regA] << cpu->registers[regB];
    break;
  case ALU_ADDI:
    cpu->registers[regA] = cpu->registers[regB] + cpu->registers[addiVal];
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
  time_t timePast = 0;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    struct timeval tv;
    time_t second;

    gettimeofday(&tv, NULL);
    second = tv.tv_sec;
    if (second != timePast)
    {
      cpu->registers[6] = 0b00000001;
      timePast = second;
    }

    unsigned char interrupts = cpu->registers[5] & cpu->registers[6];
    if (interrupts)
    {
      for (int i = 0; i < 8; i++)
      {
        int interrupt_happened = ((interrupts >> i) & 1) == 1;
        if (interrupt_happened)
        {
          handle_interupt(cpu, i);
          break;
        }
      }
    }

    unsigned char IR = cpu_ram_read(cpu, (cpu->pc));
    unsigned char operandA = cpu_ram_read(cpu, (cpu->pc + 1));
    unsigned char operandB = cpu_ram_read(cpu, (cpu->pc + 2));
    int nextOpcode = ((0b11000000 & IR) >> 6) + 1;

    // printf("ALU find IR: %d\n", (IR));
    unsigned char ALUNUM = (0b00100000 & IR) >> 5;

    if (ALUNUM == 1)
    {
      alu(cpu, IR);
    }
    else
    {

      switch (IR)
      {
      case LDI:
        handle_LDI(cpu, operandA, operandB);
        break;
      case PRN:
        handle_PRN(cpu, operandA);
        break;
      case HLT:
        handle_HLT();
        break;
      case ST:
        handle_ST(cpu, operandA, operandB);
        break;
      case JMP:
        handle_JMP(cpu, operandA);
        continue;
      case PRA:
        handle_PRA(cpu, operandA);
        break;
      case JEQ:
        handle_JEQ(cpu, operandA, nextOpcode);
        continue;
      case JNE:
        handle_JNE(cpu, operandA, nextOpcode);
        continue;
      case IRET:
        handle_IRET(cpu);
        continue;

      // Subroutine instructions
      case CALL:
        handle_CALL(cpu, operandA, (cpu->pc + nextOpcode));
        continue;
      case RET:
        handle_RET(cpu);
        continue;

      //  Stack Instructions
      case PUSH:
        handle_PUSH(cpu, cpu->registers[operandA]);
        break;
      case POP:
        handle_POP(cpu, operandA);
        break;
      default:
        printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->pc);
        exit(1);
      }
    }

    cpu->pc += nextOpcode;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  cpu->fl = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  cpu->registers[7] = 0xF4;
  memset(cpu->ram, 0, sizeof(cpu->ram));
}