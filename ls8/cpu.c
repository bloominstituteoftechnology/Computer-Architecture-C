#include "cpu.h"
#include <string.h>

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
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
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

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char _IR = cpu->ram[cpu->pc];
    // 2. Figure out how many operands this next instruction requires
    int _operands;
    if (_IR > 0b00111111 && _IR < 0b10000000)
    {
      _operands = 1;
    }
    if (_IR >= 0b10000000)
    {
      _operands = 2;
    }
    // 3. Get the appropriate value(s) of the operands
    // following this instruction
    unsigned char operandA = cpu->ram[_IR + 1];
    unsigned char operandB = cpu->ram[_IR + 2];
    // 4. switch() over it to decide on a course of action.
    switch (_IR)
    {

    case ADD: // ADD registerA registerB // --> ALU
      /*Add the value in two registers and store the result 
      in registerA.*/
      break;

    case AND: // AND registerA registerB // --> ALU
      /*Bitwise-AND the values in registerA and registerB, 
      then store the result in registerA.*/
      break;

    case CALL: // CALL register //
      /*Calls a subroutine (function) at the address stored 
      in the register.*/
      break;

    case CMP: // CMP registerA registerB //
      /*Compare the values in two registers:
      equal, set the Equal E flag to 1, otherwise set it to 0.
      A is less than registerB, set the Less-than L flag to 1, 
      otherwise set it to 0.
      A is greater than registerB, set the Greater-than G 
      flag to otherwise set it to 0.*/
      break;

    case DEC: // DEC register // --> ALU
      /*Decrement (subtract 1 from) the value in the given register.*/
      break;

    case DIV: // DIV registerA registerB // --> ALU
      /*Divide the value in the first register by the value 
      in the second, storing the result in registerA.
      If the value in the second register is 0, the system 
      should print an error message and halt.*/
      break;

    case HLT: // HLT //
      /*Halt the CPU (and exit the emulator).*/
      break;

    case INC: // INC register //
      /*Increment (add 1 to) the value in the given register.*/
      break;

    case INT: // INT register //
      /* Issue the interrupt number stored in the given register.
      This will set the _n_th bit in the IS register to the value 
      in the given register. */
      break;

    case IRET: // IRET //
      /*The following steps are executed:
      Registers R6-R0 are popped off the stack in that order.
      The FL register is popped off the stack.
      The return address is popped off the stack and stored in PC.
      Interrupts are re-enabled*/
      break;

    case JEQ: // JEQ register //
      /*If equal flag is set (true), jump to the address 
      stored in the given register.*/
      break;

    case JGE: // JGE register //
      /*If greater-than flag or equal flag is set (true), 
      jump to the address stored in the given register.*/
      break;

    case JGT: // JGT register //
      /*If greater-than flag is set (true), jump to the 
      address stored in the given register.*/
      break;

    case JLE: // JLE register //
      /*If less-than flag or equal flag is set (true), 
      jump to the address stored in the given register.*/
      break;

    case JLT: // JLT register //
      /*If less-than flag is set (true), jump to the address 
      stored in the given register.*/
      break;

    case JMP: // JMP register //
      /*Jump to the address stored in the given register.
      Set the PC to the address stored in the given register.*/
      break;

    case JNE: // JNE register //
      /*If E flag is clear (false, 0), jump to the address 
      stored in the given register.*/
      break;

    case LD: // LD registerA registerB //
      /*Loads registerA with the value 
      at the memory address stored in registerB.
      This opcode reads from memory.*/
      break;

    case LDI: // LDI register immediate //
      /*Set the value of a register to an integer.*/
      break;

    case MOD: // MOD registerA registerB //
      /*Divide the value in the first register by 
      the value in the second, storing the remainder 
      of the result in registerA.
      If the value in the second register is 0, 
      the system should print an error message and halt.*/
      break;

    case MUL: // MUL registerA registerB //
      /*Multiply the values in two registers together 
      and store the result in registerA.*/
      break;

    case NOP: // NOP //
      /*No operation. Do nothing for this instruction.*/
      break;

    case NOT: // NOT register //
      /*Perform a bitwise-NOT on the value in a register.*/
      break;

    case OR: // OR registerA registerB //
      /*Perform a bitwise-OR between the values in registerA 
      and registerB, storing the result in registerA.*/
      break;

    case POP: // POP register //
      /*Pop the value at the top of the stack into the given register.
      Copy the value from the address pointed to by SP to the given 
      register.
      Increment SP.*/
      break;

    case PRA: // PRA register //
      /*Print alpha character value stored in the given register.
      Print to the console the ASCII character corresponding to the 
      value in the register.*/
      break;

    case PRN: // PRN register //
      /*Print numeric value stored in the given register.
      Print to the console the decimal integer value that 
      is stored in the given register.*/
      break;

    case PUSH: // PUSH register //
      /*Push the value in the given register on the stack.
      Decrement the SP.
      Copy the value in the given register to the address 
      pointed to by SP.*/
      break;

    case RET: // RET //
      /*Return from subroutine.
      Pop the value from the top of the stack and store it in the PC.*/
      break;

    case SHL: // SHL //
      /*This is an instruction handled by the ALU.
      Shift the value in registerA left by the number of bits 
      specified in registerB, filling the low bits with 0.*/
      break;

    case SHR: // SHR // --> ALU
      /*Shift the value in registerA right by the number of bits 
      specified in registerB, filling the high bits with 0.*/
      break;

    case ST: // ST registerA registerB //
      /*Store value in registerB in the address stored in registerA.
      This opcode writes to memory.*/
      break;

    case SUB: // SUB registerA registerB // --> ALU
      /*Subtract the value in the second register from the 
      first, storing the result in registerA.*/
      break;

    case XOR: // XOR registerA registerB //
      /*Perform a bitwise-XOR between the values in registerA 
      and registerB, storing the result in registerA.*/
      break;

    /* you can have any number of case statements */
    default: /* Optional */
      break;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    cpu->pc = cpu->pc + _operands + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0b00000000;
  memset(cpu->ram, 0b00000000, sizeof(cpu->ram));
  memset(cpu->registers, 0b00000000, sizeof(cpu->registers));
}

unsigned char cpu_ram_read(cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(cpu *cpu, int index, unsigned char value)
{
  cpu->ram[index] = value;
  return;
}
