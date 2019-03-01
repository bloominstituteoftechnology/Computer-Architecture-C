#include "cpu.h"
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

// https://stackoverflow.com/questions/1658530/load-numbers-from-text-file-in-c
void cpu_load(struct cpu *cpu, char *argv[])//argv will come in from main according to step 8
{
  FILE *f;
  f = fopen(argv[1], "r"); //reads the second argument passed
  if(f == NULL){
    printf("File came back with null");
    exit(1);
  }else{
    printf("ARGV[1] => %s\n", argv[1]);
    int address = 0;
    char buf[256];
    // char *fgets( char *buf, int n, FILE *fp );
    // The functions fgets() reads up to n-1 characters from the input stream referenced by fp. 
    // It copies the read string into the buffer buf, appending a null character to terminate the string.
    while(fgets(buf, sizeof(buf), f) != NULL){
      unsigned long int data = strtoul(buf, NULL, 2); //converts to binary
      // printf("BUF VALUE -> %s", buf);
      // printf("DATA -> %ld\n", data);
      // You'll have to convert the binary strings to integer values to store in RAM. The
      // built-in `strtoul()` library function might help you here.
      cpu->ram[address++] = data;
    }
    fclose(f);
  }
  // TODO: Replace this with something less hard-coded
}

int cpu_ram_read(struct cpu *cpu, int position){
  return cpu->ram[position]; // they didn't really specify what they wanted this to do so hopefully this works
}

int cpu_ram_write(struct cpu *cpu, unsigned int position, unsigned char value){
  cpu->ram[position] = value;
  return cpu->ram[position];
} 

void push(struct cpu *cpu, unsigned char operandA){
  cpu->SP -= 1;
  cpu->ram[cpu->SP] = cpu->registers[operandA];   
}

unsigned char pop(struct cpu *cpu){
  return cpu_ram_read(cpu, cpu->SP++);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      // Multiply the values in two registers together and store the result in registerA.
      printf("\nMULTIPLY CALLED\n");
      cpu->registers[regA] *= cpu->registers[regB]; // sets registerA to registerA times registerB
      break;
    
    case CMP:
      if(cpu->registers[regA] == cpu->registers[regB]){
        //If they are equal, set the Equal `E` flag to 1, otherwise set it to 0.
        cpu->E = 1;
      }else{
        cpu->E = 0;
      }

    // TODO: implement more ALU ops
    

  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  printf("RUNNING : ");
  while (running) {
    unsigned int IR = cpu_ram_read(cpu, cpu->pc); //Instruction Register
    unsigned int num_of_operations = IR>>6; // bitwise shift operator I found / get the last two numbers 
    int operandA = cpu_ram_read(cpu, cpu->pc + 1); //next value
    int operandB = cpu_ram_read(cpu, cpu->pc + 2); //next next value
    // Use Switch statement

    //what is this "opcode they speak of lol what am i looking for"
  //   the machine code value of the instruction (e.g. `10000010` for `LDI`), also
  // known as the _opcode_

  //LS8-spec.md -> Instruction set -> Glossary
    switch (IR)
    {
      case LDI:/* Set the value of a register to an integer. */
        cpu->registers[operandA] = operandB;
        // cpu->pc += 3;
        cpu->pc += num_of_operations + 1;
        break;

      // case NOP://does nothing
      //   break;
      case HLT: //exits the program
        running = 0;
        break;

      case PRN:
        printf("PRN Says: %d \n", cpu->registers[operandA]);
        // cpu->pc += 2;
        cpu->pc += num_of_operations + 1;
        break;

      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        // cpu->pc += 3;
        cpu->pc += num_of_operations + 1;
        break;

      case PUSH:
        push(cpu, operandA);
        cpu->pc += num_of_operations + 1;
        break;

      case POP:
        cpu->registers[operandA] = pop(cpu);
        cpu->pc += num_of_operations + 1;
        break;

      case CALL:
        printf("\nCALL Called\n");
        //The address of the ***instruction*** _directly after_ `CALL` is
        //pushed onto the stack. This allows us to return to where we left off when the subroutine finishes executing.
        push(cpu, cpu->pc + 2);

        //  The PC is set to the address stored in the given register. 
        //  We jump to that location in RAM and execute the first instruction in the subroutine. 
        //  The PC can move forward or backwards from its current location.
        cpu->pc = cpu->registers[operandA];
        break;

      case RET:
        printf("\nRET Called\n");
        // Pop the value from the top of the stack and store it in the `PC`.
        cpu->pc = pop(cpu); 
        break; 
      
      case CMP:
        alu(cpu, CMP, operandA, operandB);
        cpu->pc += num_of_operations + 1;

      case JNE:
        break;
      default:
        break;
    }
    
    // cpu->pc += num_of_operations + 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers, use mem set
  // https://www.geeksforgeeks.org/memset-c-example/
  // At first, the PC, registers, and RAM should be cleared to zero.
  cpu->pc = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}

