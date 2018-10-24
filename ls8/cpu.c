#include "stdio.h"
#include "stdlib.h"
#include "cpu.h"
#include <string.h>

#define DATA_LEN 6

//  add functions cpu_ram_read() and cpu_ram_write()
// that access the RAM inside the struct cpu.

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR)
{
    return cpu->ram[MAR]; // cpu.ram[mar]   // returning
}
// unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
// {
//     return cpu->ram[address]; // cpu.ram[address]
// }
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char MAR, unsigned char MDR)
{
    cpu->ram[MAR] = MDR; // setting
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
    char line[1024];
    int address = 0;
    FILE *fp = fopen(filename, "r");
    // printf("%s\n", filename);

    while (fgets(line, sizeof line, fp) != NULL)
    {
        char *endchar;
        // puts(line);
        unsigned char v = strtoul(line, &endchar, 2);
        if (line == endchar)
        {
            continue;
        }
        // printf("%u\n", v);
        cpu_ram_write(cpu, address++, v);
    }
}

//     char data[DATA_LEN] = {
//         // From print8.ls8
//         0b10000010, // LDI R0,8  // LDI: load "immediate", store a value in a register, or "set this register to this value".
//         0b00000000,
//         0b00001000,
//         0b01000111, // PRN R0 // PRN: a pseudo-instruction that prints the numeric value stored in a register.
//         0b00000000,
//         0b00000001 // HLT // HLT: halt the CPU and exit the emulator.
//     };

//     int address = 0;

//     for (int i = 0; i < DATA_LEN; i++)
//     {
//         cpu->ram[address++] = data[i];
//     }

//     // TODO: Replace this with something less hard-coded
// }
// void cpu_load(struct cpu *cpu)
// {
//     char data[DATA_LEN] = {
//         // From print8.ls8
//         0b10000010, // LDI R0,8  // LDI: load "immediate", store a value in a register, or "set this register to this value".
//         0b00000000,
//         0b00001000,
//         0b01000111, // PRN R0 // PRN: a pseudo-instruction that prints the numeric value stored in a register.
//         0b00000000,
//         0b00000001 // HLT // HLT: halt the CPU and exit the emulator.
//     };

//     int address = 0;

//     for (int i = 0; i < DATA_LEN; i++)
//     {
//         cpu->ram[address++] = data[i];
//     }

//     // TODO: Replace this with something less hard-coded
// }

/**
 * ALU - An arithmetic logic unit (ALU) is a digital circuit used to perform arithmetic and logic operations. 
 Most of the operations of a CPU are performed by one or more ALUs, which load data from input registers. 
 A register is a small amount of storage available as part of a CPU. The control unit tells the ALU what
 operation to perform on that data, and the ALU stores the result in an output register. 
 The control unit moves the data between these registers, the ALU, and memory.
 */
// Enumeration (or enum) is a user defined data type in C. It is mainly used to assign names to integral constants,
// the names make a program easy to read and maintain.

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op)
    {
    case ALU_MUL:
        // TODO
        cpu->reg[regA] *= cpu->reg[regB];
        break;

        // TODO: implement more ALU ops
    case ALU_ADD:
        // TODO
        break;
    }
}

/**
 * Run the CPU
 * This is the workhorse function of the entire processor.
It needs to read the memory address that's stored in register PC, and store that result in IR, the Instruction Register. 
This can just be a local variable in cpu_run().
Some instructions requires up to the next two bytes of data after the PC in memory to perform operations on. 
Sometimes the byte value is a register number, other times it's a constant value (in the case of LDI). Using cpu_ram_read(), 
read the bytes at PC+1 and PC+2 from RAM into variables operandA and operandB in case the instruction needs them.

Then, depending on the value of the opcode, perform the actions needed for the instruction per the LS-8 spec.( switch statement)

After the handler returns, the PC needs to be updated to point to the next instruction for the next iteration of the loop in cpu_run().
The number of bytes an instruction uses can be determined from the two high bits (bits 6-7) of the instruction opcode.
 */

void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction

    while (running)
    {
        // TODO
        // 1. Get the value of the current instruction (in address PC).
        // 2. switch() over it to decide on a course of action.
        // 3. Do whatever the instruction should do according to the spec.
        // 4. Move the PC to the next instruction.
        unsigned char IR = cpu_ram_read(cpu, cpu->pc);           // pc - is the index of the currently executing instruction in the ram array
        unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1); // pc+1 - index of the byte after the insctuction
        // unsigned char operandA = cpu_ram_read(cpu, (cpu->pc + 1) & 0xff); // pc+1 - index of the byte after the insctuction
        unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
        // unsigned char operandB = cpu_ram_read(cpu, (cpu->pc + 2) & 0xff);

        //   00011001
        // & 00000011
        // ----------
        //   00000001

        // int add_to_pc = (IR >> 6) + 1;
        printf("Traces %02x: %02x %02x %02x\n", cpu->pc, IR, operandA, operandB);
        // 2 means the field width to be 2 chars
        // 0 means to pad with leading zero if necessary
        // x - hex

        switch (IR)
        {
        case LDI:
            cpu->reg[operandA] = operandB; // store value of B in a register A
            // operandA registers number that we need to store the value in
            // operandB is the value that we need to store
            // cpu->pc += 3; // jump from LDI to PRN
            break;
        case PRN:
            printf("%d\n", cpu->reg[operandA]);
            // cpu->pc += 2; // from PRN to HLT
            break;
        case HLT:
            running = 0;
            break;
        default:
            printf("unknown instruction at %02x: %02x\n", cpu->pc, IR);
            exit(2);
        case MUL:
            alu(cpu, ALU_MUL, operandA, operandB);
            break;
        }
        cpu->pc += (IR >> 6) + 1; // +1 - instruction code
    }
}

/**
 * Initialize a CPU struct
 * The cpu_init() function takes a pointer to a struct cpu and initializes it as necessary. 
 * At first, the PC, registers, and RAM should be cleared to zero. 
 * (memset() might help you clear registers and RAM.)


 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers
    cpu->pc = 0;
    // TODO: Zero registers and RAM
    memset(cpu->ram, 0, sizeof cpu->ram);
    memset(cpu->reg, 0, sizeof cpu->reg);

    // cpu->reg[7] = 0xF4; // for stacks
}
