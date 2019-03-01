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
void handle_LDI(struct cpu *cpu)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char value = cpu_ram_read(cpu, (cpu->pc + 2));
  cpu->registers[regA] = value;
};

// PRN function: Print numeric value stored in the given register
void handle_PRN(struct cpu *cpu)
{
  unsigned char regA = cpu_ram_read(cpu, (cpu->pc + 1));
  printf("%d\n", cpu->registers[regA]);
}

// HLT function: Halt the CPU (and exit the emulator)
void handle_HLT()
{
  exit(0);
}

// ST function: Store value in registerB in the address stored in registerA
// This opcode writes to memory
void handle_ST(struct cpu *cpu)
{
  unsigned char MAR = cpu->registers[cpu_ram_read(cpu, (cpu->pc + 1))];
  unsigned char MDR = cpu->registers[cpu_ram_read(cpu, (cpu->pc + 2))];
  cpu_ram_write(cpu, MAR, MDR);
}

//  JMP function: Jump to the address stored in the given register
void handle_JMP(struct cpu *cpu)
{
  unsigned char MAR = cpu->registers[cpu_ram_read(cpu, (cpu->pc + 1))];
  cpu->pc = MAR;
}

void handle_PRA(struct cpu *cpu)
{
  unsigned char regA = cpu->registers[cpu_ram_read(cpu, (cpu->pc + 1))];
  printf("%c\n", regA);
}

// JEQ function: If equal flag is set (true), jump to the address stored in the given register
void handle_JEQ(struct cpu *cpu)
{
  unsigned char MAR = cpu->registers[cpu_ram_read(cpu, (cpu->pc + 1))];
  if (cpu->fl)
  {
    printf("hi\n");
    cpu->pc = MAR;
  }
}

// ================= Interrupt Handle functions ==================
void handle_interupt(struct cpu *cpu, int location)
{
  cpu->registers[6] = 0;

  cpu_ram_write(cpu, --cpu->registers[7], cpu->pc);
  cpu_ram_write(cpu, --cpu->registers[7], cpu->fl);
  for (int i = 0; i < 7; i++)
  {
    cpu_ram_write(cpu, --cpu->registers[7], cpu->registers[i]);
  }
  cpu->pc = cpu->ram[0xF8 + location];
  cpu->registers[5] = 0;
}

void handle_IRET(struct cpu *cpu)
{
  for (int i = 6; i >= 0; i--)
  {
    cpu->registers[i] = cpu_ram_read(cpu, cpu->registers[7]++);
  }
  cpu->fl = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->pc = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->registers[5] = 1;
}

// ================= SubRoutine functions ==================
void handle_CALL(struct cpu *cpu)
{
  // Saves next PC at the correct spot
  cpu_ram_write(cpu, --cpu->registers[7], (cpu->pc + 2));

  char unsigned regAVal = cpu->registers[cpu_ram_read(cpu, (cpu->pc + 1))];
  cpu->pc = regAVal;
}

void handle_RET(struct cpu *cpu)
{
  char unsigned newPc = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->pc = newPc;
}

// ================= Stack functions ==================

void handle_PUSH(struct cpu *cpu)
{
  char unsigned MDR = cpu->registers[cpu_ram_read(cpu, (cpu->pc + 1))];
  cpu_ram_write(cpu, --cpu->registers[7], MDR);
}

void handle_POP(struct cpu *cpu)
{
  char unsigned regA = cpu_ram_read(cpu, (cpu->pc + 1));
  unsigned char value = cpu_ram_read(cpu, cpu->registers[7]++);
  cpu->registers[regA] = value;
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

  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;
  case ALU_ADD:
    cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
    break;
  case ALU_INC:
    cpu->registers[regA]++;
    break;
  case ALU_CMP:
    if (cpu->registers[regA] == cpu->registers[regB])
    {
      cpu->fl = 0b00000001;
    }
    if (cpu->registers[regA] > cpu->registers[regB])
    {
      cpu->fl = 0b00000010;
    }
    else
    {
      cpu->fl = 0b00000100;
    }
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
        handle_LDI(cpu);
        break;
      case PRN:
        handle_PRN(cpu);
        break;
      case HLT:
        handle_HLT();
        break;
      case ST:
        handle_ST(cpu);
        break;
      case JMP:
        handle_JMP(cpu);
        continue;
      case PRA:
        handle_PRA(cpu);
        break;
      case IRET:
        handle_IRET(cpu);
        continue;
      case JEQ:
        handle_JEQ(cpu);
        continue;
      // Subroutine instructions
      case CALL:
        handle_CALL(cpu);
        continue;
      case RET:
        handle_RET(cpu);
        continue;

      //  Stack Instructions
      case PUSH:
        handle_PUSH(cpu);
        break;
      case POP:
        handle_POP(cpu);
        break;
      default:
        printf("unexpected instruction 0x%02X at 0x%02X\n", IR, cpu->pc);
        exit(1);
      }
    }

    int opNum = (0b11000000 & IR) >> 6;
    cpu->pc += (opNum + 1);
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
