#include <stdio.h>
#include "cpu.h"

/**
 * read data from RAM
 */
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
    return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
    cpu->ram[address] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char* filename)
{
    // open file specified for reading "r"
    FILE* file = fopen(filename, "r");

    // error handling incase `fopen()` fails
    if (file == NULL)
    {
        printf("\nError opening file %s\n\n", filename);
        exit(1);
    }

    char* line[3];
    int index = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // save the content of each line as an `unsigned long` into `cpu->ram`
        cpu->ram[ index++ ] = strtoul(line, NULL, 2);
        // printf("\nRAM: %d\n\n", cpu->ram[index - 1]);
    }
}

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
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction

    unsigned char program_counter;
    unsigned char instruction_register;
    unsigned char operandA;
    unsigned char operandB;
    unsigned int next_instruction;

    while (running)
    {
        program_counter = cpu->pc;
        // printf("\nProgram Counter: %d\n\n", program_counter);
        instruction_register = cpu_ram_read(cpu, cpu->pc);
        // printf("\nIR: %d\n\n", instruction_register);

        switch (instruction_register >> 6)
        {
        case 1:
            // printf("1");

            next_instruction = 2;
            operandA = cpu_ram_read(cpu, cpu->pc + 1);

            switch (instruction_register)
            {
            case PRN:
                printf("\nPRN: %d\n\n", cpu->registers[operandA]);
                break;

            default:
                break;
            }

            break;

        case 2:
            // printf("\n2\n\n");

            next_instruction = 3;
            operandA = cpu_ram_read(cpu, cpu->pc + 1);
            operandB = cpu_ram_read(cpu, cpu->pc + 2);

            switch (instruction_register)
            {
            case LDI:
                cpu->registers[ operandA ] = operandB;
                break;

            case MUL:
                // printf("\n%d * %d = %d\n\n", cpu->registers[ operandA ], cpu->registers[ operandB ], cpu->registers[ operandA ] * cpu->registers[ operandB ]);
                cpu->registers[ operandA ] = cpu->registers[ operandA ] * cpu->registers[ operandB ];
                break;

            default:
                break;
            }

            break;

        default:
            // printf("\nHLT\n\n");
            running = 0;
        }

        cpu->pc = cpu->pc + next_instruction;
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers
    cpu->pc = 0;

    // TODO: Zero registers and RAM
}

// switch(instruction_register)
// {
//     case ADD:
//         printf("ADD");
//         break;

//     case AND:
//         printf("AND");
//         break;

//     case CALL:
//         printf("CALL");
//         break;

//     case CMP:
//         printf("CMP");
//         break;

//     case DEC:
//         printf("DEC");
//         break;

//     case DIV:
//         printf("DIV");
//         break;

//     case HLT:
//         printf("HLT");
//         break;

//     case INC:
//         printf("INC");
//         break;

//     case INT:
//         printf("INT");
//         break;

//     case IRET:
//         printf("IRET");
//         break;

//     case JEQ:
//         printf("JEQ");
//         break;

//     case JGE:
//         printf("JGE");
//         break;

//     case JGT:
//         printf("JGT");
//         break;

//     case JLE:
//         printf("JLE");
//         break;

//     case JLT:
//         printf("JLT");
//         break;

//     case JMP:
//         printf("JMP");
//         break;

//     case JNE:
//         printf("JNE");
//         break;

//     case LD:
//         printf("LD");
//         break;

//     case LDI:
//         printf("LDI");
//         break;

//     case MOD:
//         printf("MOD");
//         break;

//     case MUL:
//         printf("MUL");
//         break;

//     case NOP:
//         printf("NOP");
//         break;

//     case NOT:
//         printf("NOT");
//         break;

//     case OR:
//         printf("OR");
//         break;

//     case POP:
//         printf("POP");
//         break;

//     case PRA:
//         printf("PRA");
//         break;

//     case PRN:
//         printf("PRN");
//         break;

//     case PUSH:
//         printf("PUSH");
//         break;

//     case RET:
//         printf("RET");
//         break;

//     case SHL:
//         printf("SHL");
//         break;

//     case SHR:
//         printf("SHR");
//         break;

//     case ST:
//         printf("ST");
//         break;

//     case SUB:
//         printf("SUB");
//         break;

//     case XOR:
//         printf("XOR");
//         break;

//     default:
//         printf("\n%d\n", instruction_register);
// }