#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

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
        // 1. Get the value of the current instruction (in address PC).
        unsigned char ir = cpu_ram_read(cpu, cpu->pc);
        // 2. Figure out how many operands this next instruction requires
            // Not entirely needed if we use a switch statement
        // 3. Get the appropriate value(s) of the operands following this instruction
            // Operations may need up to 2 bytes after PC
        unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        // 6. Move the PC to the next instruction.
        switch (ir)
        {
            case LDI:
                // Set the value of a register to an integer
                cpu->registers[operandA] = operandB;
                cpu->pc += 3;
                break;
            case PRN:
                // Print numeric value stored in the given register
                printf("%d\n", cpu->registers[operandA]);
                cpu->pc += 2;
                break;
            case HLT:
                // Halt the CPU (and exit the emulator)
                running = 0;
                break;
            default:
                break;
        }
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->pc = 0;
    memset(cpu->ram, 0, 8 * sizeof(unsigned char));
    memset(cpu->registers, 0, 256 * sizeof(unsigned char));
    cpu->registers[7] = 0xF4;
}

/**
 * Reads from a CPU struct's RAM array
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}