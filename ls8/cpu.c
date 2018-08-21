#include "cpu.h"

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
 * CPU Instruction: Halts the CPU and exits emulator
 * 
 * @param running {int*} Pointer to cpu loop variable.
 */
void handle_HLT(int *running)
{
  *running = 0;
}

/**
 * CPU Instruction: Halts the CPU and exits emulator
 * 
 * @param registers {unsigned char[]} Register array.
 * @param reg {unsigned char} Register number.
 * @param immediate {unsigned char} value to be stored in register.
 */
void handle_LDI(unsigned char registers[], unsigned char reg, unsigned char immediate)
{
  registers[reg] = immediate;
}

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
 * Converts a decimal to a binary representation
 * 
 * @param decimal {unsigned char} integer value to convert.
 * 
 * @returns A pointer to an array where binary representation is stored.
 */
int *decimal_to_binary(unsigned char decimal)
{
  static int b[] = {0, 0, 0, 0, 0, 0, 0, 0};
  int n = decimal;
  int i = (sizeof(b) / sizeof(int)) - 1;

  while (n > 0)
  {
    b[i] = n % 2;
    n /= 2;
    --i;
  }

  return b;
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
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
}