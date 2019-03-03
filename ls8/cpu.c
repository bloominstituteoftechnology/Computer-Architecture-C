#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6

#define SP 7
unsigned int ram_address = 0;

/*
Add functions cpu_ram_read and cpu_ram_write to acces ram in struct cpu
*/

// if the read function reads from ram, it should return an unsigned char, based on how it's defined in struct cpu:
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {  // accept 2 arguments: a pointer to the cpu and an index in ram[] to access
	return cpu->ram[index];                                           // return the data held in ram at the specified index
}

// the write function doesn't need to return anything (I think) so it can be of type void:
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value) {  // accept 3 arguments: pointer to the cpu, an index to be written to, and a value to write
  	cpu->ram[index] = value;                                                     // set the value at the specified index equal to the specified value
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
  FILE *fp;
	fp = fopen(argv[1], "r");  // open file for reading
	char line[1024];
	char *converted_line;

  // Error handling in case file doesn't open or doesn't exist:
  if(fp == NULL) {
		fprintf(stderr, "File %s cannot be opened or does not exist. \n", argv[1]);
		exit(1);
	}

  while(fgets(line, sizeof(line), fp) != NULL) {  // fgets returns line -- chars are read from fp and stored in line, max num of chars read == sizeof(line)
			
   	unsigned char byte = strtoul(line, &converted_line, 2); // converts line from binary string to integer value
			                                                      // unsigned long int strtoul(const char *str, char **endptr, int base)

    if(converted_line == line) {
      continue;
    }
      				
		cpu->ram[ram_address++] = byte;
  }
	
	fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->reg[regA] = cpu->reg[regA] * cpu->reg[regB];
      break;

    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
      break;
    
    case ALU_CMP:
      if(cpu->reg[regA] == cpu->reg[regB]){
      	cpu->E = 1;
      }  
      else {
      	cpu->E = 0;
      }

      if(cpu->reg[regA] > cpu->reg[regB]){
        cpu->G = 1;
      }
      else {
        cpu->G = 0;
      }

      if(cpu->reg[regA] < cpu->reg[regB]){
        cpu->L = 1;
      }
      else {
        cpu->L = 0;
      }
      break;
    
    default:
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
  unsigned char PC = cpu->PC; // shorter variable to use below

  while (running) {
    
    // TODO
    // 1. Get the value of the current instruction (in address 'PC'):
    unsigned char current_instruction = cpu_ram_read(cpu, PC);    // current_instruction == IR
    // 2. Figure out how many operands this next instruction requires:
    unsigned int num_operands = current_instruction >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction:
    unsigned operandA = cpu_ram_read(cpu, (PC + 1));
	  unsigned operandB = cpu_ram_read(cpu, (PC + 2));

    int shift = (num_operands) + 1;

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch(current_instruction) {
      // HLT -- Halt the CPU and exit the emulator:
      case HLT:
        running = 0;  // Stop the 'while (running)' loop
        break;

      // LDI -- Set a specified register to a specified integer value
      // consider it as taking 2 arguments: 'register' and 'immediate'
      // ??? in this case, set the next instruction to the instruction 2 steps ahead:
      case LDI:
        cpu->reg[operandA] = operandB;
        PC += shift;
		    break;

      // PRN -- Print numeric value stored in the given register:
      case PRN:
        printf("%d \n", cpu->reg[operandA]);
        PC += shift;
		    break;
      
      case POP:
        cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]++);
        if (cpu->reg[SP] > 255) {
          cpu->reg[SP] = 0xF4;
        }
        PC += shift;
        break;

      case PUSH:
        if (--cpu->reg[SP] <= ram_address) {
          fprintf(stderr, "Warning: Stack overflow.\n");
          exit(1);
        }
        cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
        PC += shift;
        break;
      
      case CALL:
        cpu->reg[SP]--;  // decrement the value stored at reg[SP] (the stack pointer) by 2
        cpu_ram_write(cpu, cpu->reg[SP], PC + 2);
        PC = cpu->reg[operandA];
        break;
      
      case RET:
        PC = cpu_ram_read(cpu, cpu->reg[SP]);
		    cpu->reg[SP]++;
		    break;

      case JMP:
		    PC = cpu->reg[operandA];
		    break;

      case JNE:
        if(cpu->E == 0) {
            PC = cpu->reg[operandA];
        } else {
          PC += shift;
        }
		    break;

      case JEQ:
		    if(cpu->E == 1) {
          PC = cpu->reg[operandA];
        } else {
			    PC += shift;
		    }
        break;

      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        PC += shift;
		    break;

      case MUL:
      	alu(cpu, ALU_MUL, operandA, operandB);
		    PC += shift;
		    break;
      
      case ADD:
      	alu(cpu, ALU_ADD, operandA, operandB);
		    PC += shift;
		    break;

      default:
        break;
    }
    // 6. Move the PC to the next instruction.
    // PC += num_operands + 1; 
    // *** To implement CALL and RET, I need to move this into the individual instruction handlers b/c CALL and RET move the PC differently
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0x00;

  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));

  cpu->reg[SP] = 0xF4;  // The stack pointer is always stored in the register at reg[SP]
                        // upon init, it's value is set to a number (0xF4) that corresponds to an index in ram[]
                        // the stack should start at the top of memory (at a high address) and grow downward as things are pushed on

  cpu->E = 0;   
  cpu->G = 0;
  cpu->L = 0; 
}
