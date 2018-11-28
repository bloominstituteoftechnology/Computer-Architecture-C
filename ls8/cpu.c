#include "cpu.h"

#define DATA_LEN 6

/** Add RAM functions */
unsigned char cpu_ram_read(struct cpu *cpu, int index) {
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char value) {
  cpu->ram[index] = value;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char, argv[])
{
  char data[DATA_LEN] = {
    // From print8.ls8
    //0b10000010, // LDI R0,8
    //0b00000000,
    //0b00001000,
    //0b01000111, // PRN R0
    //0b00000000,
    //0b00000001  // HLT
  };
  FILE *fp = fopen(argv[1], "r")
  int address = 0;
  char new[8192];

  while (fgets(new, sizeofn new, fp) != NULL) {
    cpu->ram[address++] = strtoul(new, NULL, 2);
  }
  fclose(fp);

  //for (int i = 0; i < DATA_LEN; i++) {
   // cpu->ram[address++] = data[i];
 // }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
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
    unsigned char ir = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC + 2);
    // 2. switch() over it to decide on a course of action.
    switch (ir)
    {
    case LDI:
      cpu->registers[operandA] = operandB;
      cpu->PC += 3;
      break;
    case PRN:
       printf("%d\n", cpu->registers[operandA]);
      cpu->PC += 2;
      break;
    case HLT:
      running = 0;
      cpu->PC++;
      break;
      case MUL:
      break;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  memset(cpu->ram,0,sizeof(cpu->ram));
  memset(cpu->registers,0,sizeof(cpu->registers));
}
