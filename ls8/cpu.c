#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

// Declare an array of pointers to functions and initialize them to NULL
void (*branchTable[256])(struct cpu *cpu, unsigned char, unsigned char) = {0};

// Return the value in memory
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// Write to memory with the given address / value
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

// Pops an item off the stack and returns it
unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char value = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP]++;

  return value;
}

// Pushes an item to the stack
void cpu_push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[SP]--;
  cpu->ram[cpu->registers[SP]] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *file = fopen(filename, "r");
  char line[1024];

  int address = 0;

  // Writes the value of each line into memory
  while (fgets(line, sizeof line, file) != NULL)
  {
    if (line[0] == '\n' || line[0] == '#')
    {
      continue;
    }

    cpu_ram_write(cpu, address++, strtol(line, NULL, 2));
  }

  fclose(file);
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

    // TODO: implement more ALU ops
  }
}

struct termios orig_termios;

void reset_terminal_mode()
{
  tcsetattr(0, TCSANOW, &orig_termios);
}

struct termios orig_termios;

void disableRawMode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode()
{
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  struct termios raw = orig_termios;

  raw.c_lflag &= ~(ECHO);
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit()
{
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
  int r;
  unsigned char c;
  if ((r = read(0, &c, sizeof(c))) < 0)
  {
    return r;
  }
  else
  {
    return c;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  enableRawMode();
  struct timeval tval;
  long next = 0;

  while (cpu->running)
  {
    // Checks to see if a key has been pressed and sets the IS register
    // Writes to the memory address 0xF4 with the value of the key which was pressed
    if (kbhit())
    {
      cpu->registers[IS] |= (0x1 << 1); // Keyboard interrupt
      cpu_ram_write(cpu, 0xF4, getch());
    }

    gettimeofday(&tval, NULL);

    // Timer - checks if a second has elapsed and sets the IS register
    if (tval.tv_sec == next)
    {
      cpu->registers[IS] |= (0x1 << 0); // Timer interrupt
    }

    next = tval.tv_sec + 1;

    // Checks to see if there is an interrupt
    if (cpu->interrupts)
    {
      unsigned char maskedInterrupts = cpu->registers[IS] & cpu->registers[IM];

      for (int i = 0; i < 8; i++)
      {
        if (((maskedInterrupts >> i) & 1) == 1)
        {
          // Disable interrupts
          cpu->interrupts = 0;
          // Clear bit in IS register
          cpu->registers[IS] &= ~(1 << i);
          // Push PC on to the stack
          cpu_push(cpu, cpu->PC);
          // Push registers R0-R6 on to the stack
          for (int i = 0; i <= 6; i++)
          {
            cpu_push(cpu, cpu->registers[i]);
          }
          // Set PC to the appropriate handler in the interrupt vector table
          cpu->PC = cpu->ram[0xF8 + i];
        }
      }
    }

    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1) & 0xff;
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2) & 0xff;

    // Declare handler to be a pointer to a function
    void (*handler)(struct cpu * cpu, unsigned char, unsigned char);

    // Initialize handler to one of the handler functions in the branchTable array
    handler = branchTable[IR];

    // Checks if handler doesn't exist
    if (handler == NULL)
    {
      printf("Unknown instruction: %02x\n", IR);
      return;
    }

    // Call the function given by the branchTable array
    handler(cpu, operandA, operandB);

    // Move on to the next instruction
    int changed = (IR >> 4) & 1;
    if (!changed)
    {
      cpu->PC += (IR >> 6) + 1;
    }
  }
}

// Handler Functions

void handle_LDI(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = operandB;
}

void handle_POP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandB;
  cpu->registers[operandA] = cpu_pop(cpu);
}

void handle_PUSH(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandB;
  cpu_push(cpu, cpu->registers[operandA]);
}

void handle_MUL(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  alu(cpu, ALU_MUL, operandA, operandB);
}

void handle_PRN(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandB;
  printf("%d\n", cpu->registers[operandA]);
}

void handle_HLT(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandA;
  (void)operandB;
  cpu->running = 0;
}

void handle_CALL(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandB;
  cpu_push(cpu, cpu->PC + 2);
  cpu->PC = cpu->registers[operandA];
}

void handle_ADD(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  alu(cpu, ALU_ADD, operandA, operandB);
}

void handle_RET(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandA;
  (void)operandB;
  cpu->PC = cpu_pop(cpu);
}

void handle_ST(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu_ram_write(cpu, cpu->registers[operandA], cpu->registers[operandB]);
}

void handle_JMP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandB;
  cpu->PC = cpu->registers[operandA];
}

void handle_PRA(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandB;
  printf("%c\n", cpu->registers[operandA]);
}

void handle_IRET(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  (void)operandA;
  (void)operandB;
  // Set registers back to where they were before the interrupt
  for (int i = 6; i >= 0; i--)
  {
    cpu->registers[i] = cpu_pop(cpu);
  }
  // Set PC back to where it was before the interrupt
  cpu->PC = cpu_pop(cpu);
  // Re-enable interrupts
  cpu->interrupts = 1;
}

void handle_LD(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  cpu->registers[operandA] = cpu_ram_read(cpu, cpu->registers[operandB]);
}

void handle_CMP(struct cpu *cpu, unsigned char operandA, unsigned char operandB)
{
  if (cpu->registers[operandA] == cpu->registers[operandB])
  {
    cpu->FL |= (0x1 << 0);
    // Clear other bits
    cpu->FL &= ~(0x1 << 1);
    cpu->FL &= ~(0x1 << 2);
  }
  else if (cpu->registers[operandA] > cpu->registers[operandB])
  {
    cpu->FL |= (0x1 << 1);
    // Clear other bits
    cpu->FL &= ~(0x1 << 2);
    cpu->FL &= ~(0x1 << 0);
  }
  else
  {
    cpu->FL |= (0x1 << 2);
    // Clear other bits
    cpu->FL &= ~(0x1 << 1);
    cpu->FL &= ~(0x1 << 0);
  }
  printf("%d\n", cpu->FL);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;
  cpu->running = 1;
  cpu->interrupts = 1;

  // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);

  // Point SP to 0xF4 since stack is empty
  cpu->registers[SP] = 0xF4;

  // Initialize branchTable with the handler functions
  branchTable[LDI] = handle_LDI;
  branchTable[MUL] = handle_MUL;
  branchTable[PRN] = handle_PRN;
  branchTable[HLT] = handle_HLT;
  branchTable[POP] = handle_POP;
  branchTable[PUSH] = handle_PUSH;
  branchTable[CALL] = handle_CALL;
  branchTable[ADD] = handle_ADD;
  branchTable[RET] = handle_RET;
  branchTable[ST] = handle_ST;
  branchTable[JMP] = handle_JMP;
  branchTable[PRA] = handle_PRA;
  branchTable[IRET] = handle_IRET;
  branchTable[LD] = handle_LD;
  branchTable[CMP] = handle_CMP;
}
