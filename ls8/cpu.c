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
        0b10000010, // LDI R1,9
        0b00000001,
        0b00001001,
        0b10100010,// MUL R0,R1
        0b00000000,
        0b00000001,
        0b01000111, // PRN R0
        0b00000000,
        0b00000001, // HLT
      
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


// fgets
// char *fgets(char *str, int n, FILE *stream)
// str − This is the pointer to an array of chars where the string read is stored.

// n − This is the maximum number of characters to be read (including the final null-character). Usually, the length of the array passed as str is used.

// stream − This is the pointer to a FILE object that identifies the stream where characters are read from.

// strtoul
// unsigned long int strtoul(const char *str, char **endptr, int base)
// str − This is the string containing the representation of an unsigned integral number.

// endptr − This is the reference to an object of type char*, whose value is set by the function to the next character in str after the numerical value.

// base − This is the base, which must be between 2 and 36 inclusive, or be the special value 0.





// Example Code for reference
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