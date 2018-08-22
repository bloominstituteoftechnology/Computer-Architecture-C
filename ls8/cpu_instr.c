#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "cpu_instr.h"

/**
 * CPU Instruction: Adds values in regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_ADD(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_ADD, opA, opB);
}

/**
 * CPU Instruction: Calls subroutine stored at address in register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_CALL(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[TMP] = cpu->pc + 1;
  handle_PUSH(cpu, TMP, '\0');
  cpu->pc = cpu->registers[opA];
  cpu_ram_read(cpu->ram, cpu->pc, &cpu->ir);
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
 * CPU Instruction: Multiplies values in regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: immediate.
 */
void handle_MUL(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_MUL, opA, opB);
}

/**
 * CPU Instruction: Pop first value out of stack and store in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_POP(struct cpu *cpu, unsigned char opA, unsigned char opB){
  cpu->registers[opA] = cpu->ram[cpu->registers[SP]];
  cpu->registers[SP] += 1;
}

/**
 * CPU Instruction: Prints numeric value stored in given register
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_PRN(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  printf("%d\n", cpu->registers[opA]);
}

/**
 * CPU Instruction: Push given value on to stack
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register.
 * @param opB {unsigned char} Operand B: --
 */
void handle_PUSH(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->registers[SP] -= 1;
  cpu->ram[cpu->registers[SP]] = cpu->registers[opA];
}

/**
 * CPU Instruction: Returns from subroutine using address in stack
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: --
 * @param opB {unsigned char} Operand B: --
 */
void handle_RET(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  handle_POP(cpu, TMP, '\0');
  cpu->pc = cpu->registers[TMP];
  cpu_ram_read(cpu->ram, cpu->pc, &cpu->ir);
}

/**
 * Loads CPU instructions into branch table
 * 
 * @param bt {handler*} Pointer to an array of function pointers.
 */
void load_cpu_instructions(handler *bt)
{
  bt[ADD] = handle_ADD;
  bt[CALL] = handle_CALL;
  bt[LDI] = handle_LDI;
  bt[MUL] = handle_MUL;
  bt[POP] = handle_POP;
  bt[PRN] = handle_PRN;
  bt[PUSH] = handle_PUSH;
  bt[RET] = handle_RET;
}