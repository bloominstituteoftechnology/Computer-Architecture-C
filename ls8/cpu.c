#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
};
  
void cpu_ram_write(){
  //assign that index above to value
};

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
  // switch (op) {
  //   case ALU_HLT:
  //     break;
  //   case ALU_LDI:
  //     regB = regA;
  //     break;
  //   case ALU_PRN:
  //     printf("this is number is in the next register %i", regA);
  //     break;
  //   // TODO: implement more ALU ops
  // }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    int IR = cpu->ram[cpu->PC];//this is the slot in ram

    unsigned int value1 = cpu_ram_read(cpu, cpu->PC+1); //the register index  where the second should be stored 
    unsigned int value2 = cpu_ram_read(cpu, cpu->PC+2);//the second number

    //value at register zero   

    // 1. Get the value of the current instruction (in address PC).
    switch(IR){
      case LDI:
        //set the value of a register to an integer
        cpu->reg[value1] = value2;
        cpu->PC+=3;
        break;
      case PRN:
        printf("The number printed is %u\n", cpu->reg[value1]);
        cpu->PC += 2;
        break;
      case HLT:
        exit(0); //terminates the whole program insetead of exiting switch... exit is kinda opposite ish 
        break;
      default: 
        exit(1);
    }
    // 2. switch() over it to decide on a course of action.
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
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram)); // 0 is what it is assigned to. 
  memset(cpu->reg, 0, sizeof(cpu->reg));
}