#include "cpu.h"
#include "cpuinstructions.h"

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Reads from RAM address and stores value into store
 * 
 * @param ram {unsigned char[]} Memory array.
 * @param address {unsigned char} Address for memory array.
 * @param store {unsigned char*} Pointer to location value at memory address should be stored.
 */
void cpu_ram_read(unsigned char ram[], unsigned char address, unsigned char *store)
{
  *store = ram[address];
}

/**
 * Writes data to RAM address 
 * 
 * @param ram {unsigned char[]} Memory array.
 * @param address {unsigned char} Address for memory array.
 * @param data {unsigned char} data to be stored at memory address.
 */
void cpu_ram_write(unsigned char ram[], unsigned char address, unsigned char data)
{
  ram[address] = data;
}

/**
 * Gets number of operands in instruction, sets them to variables and updates PC
 * 
 * @param opA {unsigned char*} Pointer to operandA variable.
 * @param opB {unsigned char*} Pointer to operandB variable.
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void get_operands(unsigned char *opA, unsigned char *opB, struct cpu *cpu)
{
  unsigned char ops = cpu->ir >> 6;

  if (ops > 0)
  {
    cpu->pc++;
    cpu_ram_read(cpu->ram, cpu->pc, opA);
  }

  if (ops > 1)
  {
    cpu->pc++;
    cpu_ram_read(cpu->ram, cpu->pc, opB);
  }
}

/**
 * Initialize a CPU struct
 * 
 * Initializes the PC and other special registers. -> TODO
 * Zeros registers and RAM. -> TODO
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0b00000000;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
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
 * Run the CPU
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1;

  while (running) {
    unsigned char operandA = '\0';
    unsigned char operandB = '\0';
    cpu_ram_read(cpu->ram, cpu->pc, &cpu->ir);
    get_operands(&operandA, &operandB, cpu);

    switch (cpu->ir)
    {
      case HLT:
        handle_HLT(&running);
        break;
      case LDI:
        handle_LDI(cpu->registers, operandA, operandB);
        break;
      case PRN:
        handle_PRN(cpu->registers, operandA);
        break;
    }

    cpu->pc++;
  }
}