#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
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
        // printf("1: Entered While Loop\n"); //==================
        // === STEP 3 TODO ===
        // 1. Get the value of the current instruction (in address PC).
        // 2. switch() over it to decide on a course of action.
        // 3. Do whatever the instruction should do according to the spec.
        // 4. Move the PC to the next instruction.

        //Unstructure variables from cpu struct
        unsigned char IR = cpu->IR;
        unsigned char MAR = cpu->MAR; // operandA == MAR
        unsigned char MDR = cpu->MDR; // operandB == MDR

        // Set IR using cpu_ram_read
        IR = cpu_ram_read(cpu, PC);

        //get numArgs using bitwise rightshift on IR
        int numArgs = (IR >> 6) + 1;
        // printf("numArgs: %d\n", numArgs);

        // Switch to set MDR and MAR so we only read when we need to
        switch (numArgs)
        {
        case 3:
        // printf("2: MDR\n");
            MDR = cpu_ram_read(cpu, PC + 2);
        case 2:
        // printf("2.5: MAR\n");
            MAR = cpu_ram_read(cpu, PC + 1);
            break;
        }

        switch (IR)
        {
        // STEP 4
        case HLT:
            // printf("4: HLT\n");
            running = 0;
            break;
        // STEP 5
        case LDI:
            // printf("5: LDI\n");
            registers[MAR] = MDR;
            PC += numArgs;
            break;
        // STEP 6
        case PRN:
            // printf("6: PRN\n");
            printf("%d\n", registers[MAR]);
            PC += numArgs;
            break;
        default:
            // from solution lecture
            printf("unknown instructions: %02x at %p\n", IR, cpu->ram);
            running = 0;
            exit(2);
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
    // for loop to put 0 value into every position ..... or just use memset
    memset(cpu->registers, 0, sizeof cpu->registers);
    memset(cpu->ram, 0, sizeof cpu->ram);
}
