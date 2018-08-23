#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "cpu_instr.h"

// TODO: Try variadic functions to reduce number of unused parameters

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
 * CPU Instruction: Decrements the value in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_DEC(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_DEC, opA, opB);
}

/**
 * CPU Instruction: Divides values in regA and regB and stores result in regA
 * 
 * Issues error and exits program if regB is 0
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_DIV(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if(cpu->registers[opB] == 0){
    printf("error: dividing by zero");
    exit(1);
  }
  alu(cpu, ALU_DIV, opA, opB);
}

/**
 * CPU Instruction: Increments the value in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_INC(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_INC, opA, opB);
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
 * CPU Instruction: Divides values in regA and regB and stores remainder in regA
 * 
 * Issues error and exits program if regB is 0
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_MOD(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  if (cpu->registers[opB] == 0)
  {
    printf("error: dividing by zero");
    exit(1);
  }
  alu(cpu, ALU_MOD, opA, opB);
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
 * TODO: Make use of INC instruction
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
 * TODO: Make use of DEC instruction
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
 * CPU Instruction: Stores value in regB to address in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A
 * @param opB {unsigned char} Operand B: register B
 */
void handle_ST(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  cpu->mar = cpu->registers[opA];
  cpu->mdr = cpu->registers[opB];
  cpu_ram_write(cpu);
}

/**
 * CPU Instruction: Subtracts values in regA and regB and stores result in regA
 * 
 * @param cpu {struct cpu*} Pointer to a cpu struct.
 * @param opA {unsigned char} Operand A: register A.
 * @param opB {unsigned char} Operand B: register B.
 */
void handle_SUB(struct cpu *cpu, unsigned char opA, unsigned char opB)
{
  alu(cpu, ALU_SUB, opA, opB);
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
  bt[DEC] = handle_DEC;
  bt[DIV] = handle_DIV;
  bt[INC] = handle_INC;
  bt[LDI] = handle_LDI;
  bt[MOD] = handle_MOD;
  bt[MUL] = handle_MUL;
  bt[POP] = handle_POP;
  bt[PRN] = handle_PRN;
  bt[PUSH] = handle_PUSH;
  bt[RET] = handle_RET;
  bt[ST] = handle_ST;
  bt[SUB] = handle_SUB;
}