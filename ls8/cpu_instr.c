#include <stdio.h>
#include "cpu_instr.h"

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
 * CPU Instruction: Sets value of register to an integer
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
 * CPU Instruction: Prints numeric value stored in given register
 * 
 * @param registers {unsigned char[]} Register array.
 * @param reg {unsigned char} Register number.
 */
void handle_PRN(unsigned char registers[], unsigned char reg)
{
  printf("%d\n", registers[reg]);
}