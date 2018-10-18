#include "cpu.h"
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#define DATA_LEN 6

void cpu_load(struct cpu *cpu, char *filename)
{
    char pathname[30] = "./examples/";
    char line[1024];
    int address = 0;
    strcat(pathname, filename);
    strcat(pathname, ".ls8");
    FILE *fp;
    fp = fopen(pathname, "r");

    if (fp == NULL)
    {
        puts("couldnt open file");
        exit(0);
    }
    while (fgets(line, sizeof line, fp) != NULL)
    {
        char *endpr;
        unsigned char b = strtoul(line, &endpr, 2);
        if (endpr == line)
        {
            continue;
        }
        cpu->ram[address++] = b;
    }
    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op)
    {
    case ALU_MUL:
        cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
        break;
    case ALU_ADD:
        cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
        break;
    }
}
char cpu_ram_read(struct cpu *cpu, int index)
{
    return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, int index, char value)
{
    cpu->ram[index] = value;
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction

    while (running)
    {
        unsigned char IR = cpu_ram_read(cpu, cpu->PC);
        unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1));
        unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2));
        int shiftIndex = (IR >> 6) + 1;
        int i;
        char next = 1;

        printf("TRACE: %02X: %02X %02X %02X |", cpu->PC, IR, operandA, operandB);
        for (int i = 0; i < 8; i++)
        {
            printf(" %02X", cpu->reg[i]);
        }
        printf("\n");
        switch (IR)
        {
        case LDI:
            cpu->reg[operandA] = operandB;
            break;
        case PRN:
            printf("%d\n", cpu->reg[operandA]);
            break;
        case MUL:
            alu(cpu, ALU_MUL, operandA, operandB);
            break;
        case HLT:
            running = 0;
            break;
        case ADD:
            alu(cpu, ALU_ADD, operandA, operandB);
            break;
        case PUSH:
            i = --cpu->reg[7];
            cpu->ram[i] = cpu->reg[operandA];
            break;
        case POP:
            i = cpu->reg[7];
            cpu->reg[operandA] = cpu->ram[i];
            cpu->reg[7]++;
            break;
        case RET:
            i = cpu->reg[7];
            cpu->PC = cpu->ram[i];
            cpu->reg[7]++;
            next = 0;

            break;
        case CALL:
            i = --cpu->reg[7];
            cpu->ram[i] = cpu->PC + shiftIndex;
            cpu->PC = cpu->reg[operandA];
            next = 0;
            break;
        default:
            break;
        }

        if (next)
        {
            cpu->PC += shiftIndex;
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
    memset(cpu->reg, 0, sizeof(cpu->reg));
    memset(cpu->ram, 0, sizeof(cpu->ram));
    cpu->reg[7] = 0xF4;
}
