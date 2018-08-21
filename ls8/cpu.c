#include "cpu.h"
#include <stdio.h>
#include <string.h>

/**
 * THIS IS WHAT WILL HAPPEN LATER
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
    const int DATA_LEN = 6;
    char data[6] = {
        // From print8.ls8
        0b10000010, // LDI R0,8
        0b00000000,
        0b00001000,
        0b01000111, // PRN R0
        0b00000000,
        0b00000001 // HLT
    };

    int address = 0;

    // PUTTING THAT STUFF ABOVE INTO RAM
    for (int i = 0; i < DATA_LEN; i++)
    {
        cpu->ram[address++] = data[i];
    }

    // TODO: Replace this with something less hard-coded
}

// STEP 2
unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
    return cpu->ram[index];
}

// should this return something or just write?
unsigned char cpu_ram_write(struct cpu *cpu, int address, unsigned char value)
{
    return cpu->ram[address] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op)
    {
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
    unsigned char PC = cpu->PC;
    unsigned char *registers = cpu->registers;

    while (running)
    {
        // === STEP 3 TODO ===
        // 1. Get the value of the current instruction (in address PC).
        unsigned char IR = cpu_ram_read(cpu, PC);
        unsigned char operandA = cpu_ram_read(cpu, PC + 1);
        unsigned char operandB = cpu_ram_read(cpu, PC + 2);

        // 2. switch() over it to decide on a course of action.
        switch (IR)
        {
        // STEP 4
        case HLT:
            running = 0;
            break;
        // STEP 5
        case LDI:
            registers[operandA] = operandB;
            PC += 3;
            break;

        }
        // 3. Do whatever the instruction should do according to the spec.
        // 4. Move the PC to the next instruction.
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers
   // cpu->PC = 0;

    // TODO: Zero registers and RAM
}
