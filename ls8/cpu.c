#include "cpu.h"

#define DATA_LEN 6


// CPU Ram read
unsigned char cpu_ram_read(unsigned char index, struct cpu *cpu) 
{
  return cpu->ram[index];
}
// CPU Ram write
void cpu_ram_write(unsigned char index, struct cpu *cpu, unsigned char value) 
{
  cpu->ram[index] = value;
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
  int running = 1; // True until we get a HLT instruction
  unsigned char ir;
  unsigned char register_index;
  int num;
  
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    ir = cpu_ram_read(cpu->pc, cpu);
    // 2. switch() over it to decide on a course of action.
    switch(ir) {
      case HLT: // HLT
        running = 0;
        break;
      case PRN:
        register_index = cpu_ram_read(cpu->pc + 1, cpu);
        printf("register value %d\n", cpu->registers[register_index]);
        cpu->pc += 2;
        break;
      case LDI:   
        num = cpu_ram_read(cpu->pc + 2, cpu); 
        cpu->registers[cpu_ram_read(cpu->pc + 1, cpu)] = num;
        cpu->pc += 3;
        break; 
    }
      /*
      RAM = {
        // From print8.ls8
        0b10000010, // LDI R0,8   Index/Address 0
        0b00000000,               Index/Address 1
        0b00001000,               Index/Address 2
        0b01000111, // PRN R0     Index/Address 3
        0b00000000,               Index/Address 4
        0b00000001  // HLT        Index/Address 5
      };
      cpu->pc = 3; // keeps track of which index _in RAM_ we are in
      When we start the program, we're going to read the instruction _in RAM+ that's at index PC
      Since cpu->pc == 3, we read the PRN at index/address 3
    // 3. Do whatever the instruction should do according to the spec.
    */
    // 4. Move the PC to the next instruction.
    // cpu->pc + 1
    // int number = 1;
    // switch (number) {
    //   case 1:
    //     printf("lonely");
    //     break;
    //   case 7:
    //     printf("lucky");
    //     break;
    //   case 69; 
    //     printf("--cking"); 
    //     break;
    //   default:
    //     printf("wtf");
    //     break;
    // }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu = calloc(1, sizeof(struct cpu));
  // TODO: Zero registers and RAM
  cpu->pc = 0;
  memset(&cpu->registers, 0, 8);
  memset(&cpu->ram, 0, 256);
  
}


// switch(expression) {

//    case constant-expression  :
//       statement(s);
//       break; /* optional */
	
//    case constant-expression  :
//       statement(s);
//       break; /* optional */
  
//    /* you can have any number of case statements */
//    default : /* Optional */
//    statement(s);
// }


// memset() is used to fill a block of memory with a particular value.
// The syntax of memset() function is as follows :

// // ptr ==> Starting address of memory to be filled
// // x   ==> Value to be filled
// // n   ==> Number of bytes to be filled starting 
// //         from ptr to be filled
// void *memset(void *ptr, int x, size_t n);