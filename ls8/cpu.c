#include "cpu.h"

#define DATA_LEN 6

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
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
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
  unsigned char IR, operandA, operandB, operandC, operandD;
  int instruction_index;


// gives LDI

  int running = 1; // True until we get a HLT instruction

//  operandA = cpu_ram_read(cpu, cpu->pc + 1); // r0
//  operandB = cpu_ram_read(cpu, cpu->pc + 2); // 8 value


//  printf("A: %u\n B: %u\n", operandA, operandB);
  // printf("%d \n", instruction_index);
  // printf("%u \n", IR);

  // IR = cpu_ram_read(cpu, instruction_index);

  instruction_index = cpu->pc;


  while (running) {
  IR = cpu_ram_read(cpu, instruction_index);


    // TODO
    // 1. Get the value of the current instruction (in address PC).

     // r0

    // 8 value



    // 2. switch() over it to decide on a course of action.
     switch(IR) {

      case LDI:
      operandA = cpu_ram_read(cpu, instruction_index + 1); 
      operandB = cpu_ram_read(cpu, instruction_index + 2);

        cpu->registers[operandA] = operandB;
        instruction_index += 3;
        break;

       case PRN:
        operandA = cpu_ram_read(cpu, instruction_index + 1); 

        printf("%d\n", cpu->registers[operandA]);
        instruction_index += 2;
        break;

      case HLT:
        running = 0;
        break;


       default:
        printf("Nothing to run \n");
        running = 0;
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
