#include "cpu.h"

#define DATA_LEN 50

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar )
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu,unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
0b10000010, // # LDI R0,8
0b00000000, //
0b00001000, //
0b10000010, // # LDI R1,9
0b00000001, //
0b00001001, //
0b10100010, // # MUL R0,R1
0b00000000, //
0b00000001, //
0b01000111, // # PRN R0
0b00000000, //
0b00000001, // # HLT
 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
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
  unsigned char IR, operandA, operandB;
  int instruction_index;


// gives LDI

  int running = 1; // True until we get a HLT instruction

  instruction_index = cpu->pc; // initialize index of instruction

  while (running) {
  IR = cpu_ram_read(cpu, instruction_index); // set IR to current value of instruction index

     switch(IR) { // switch based on IR's instruction

      case LDI: //Set the value of a register to an integer.
        operandA = cpu_ram_read(cpu, instruction_index + 1); // sets operandA to the next line below instruction
        operandB = cpu_ram_read(cpu, instruction_index + 2); // if two instructions sets operandB to the 2nd line below instruction

          cpu->registers[operandA] = operandB;
          instruction_index += 3; // increments instruction index to next instruction line
          break; // breaks switch and re-runs loops

      case PRN: // print value of integer saved to register address
        operandA = cpu_ram_read(cpu, instruction_index + 1); 

          printf("%d\n", cpu->registers[operandA]);
          instruction_index += 2; // increments instruction index to next instruction line
          break; // breaks switch and re-runs loops

      case MUL:
        operandA = cpu_ram_read(cpu, instruction_index + 1); 
        operandB = cpu_ram_read(cpu, instruction_index + 2); 

          cpu->registers[operandA] = cpu->registers[operandA]*cpu->registers[operandB];
          instruction_index += 3; // increments instruction index to next instruction line
          break;
          
      case HLT:
        running = 0; // kill while loop
          break;


       default:
        printf("Nothing to run \n");
        running = 0; // kill while loop
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
  cpu->pc = 0;

  // TODO: Zero registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);
}
