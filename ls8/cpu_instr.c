#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "cpu_instr.h"

/**
 * CPU Instruction: Halts the CPU and exits emulator
 * 
 * @param status {int*} Pointer to cpu loop variable.
 */
void handle_HLT(int *status)
{
  *status = 0;
}

/**
 * CPU Instruction: Sets value of register to an integer
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: immediate.
 */
void handle_LDI(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[opA] = opB;
}

/**
 * CPU Instruction: Prints numeric value stored in given register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: N/A
 */
void handle_PRN(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  printf("%d\n", cpu->registers[opA]);
}

/**
 * Loads CPU instructions into branch table
 * 
 * @param bt {handler*} Pointer to an array of function pointers.
 */
void load_cpu_instructions(handler *bt)
{
  bt[LDI] = handle_LDI;
  bt[PRN] = handle_PRN;
}