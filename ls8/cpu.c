#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define DATA_LEN 6


/* cpu_ram_read/cpu_ram_write  */

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  	//printf("reading value: %d from RAM at index: %d \n", cpu->ram[index], index);
	return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value)
{
	//printf("writing to RAM %d \n", value);
  	cpu->ram[index] = value;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  // TODO: Replace this with something less hard-coded
  
	FILE *fp;
	fp = fopen(argv[1], "r");
	char line[1024];
	unsigned char address = 0;

	if(fp == NULL){
		fprintf(stderr, "Cannot open the file %s \n", argv[1]);
		exit(1);
	}
	
	while(fgets(line, sizeof(line), fp)!= NULL){
		        
			//char *endptr;
   			unsigned char byte = strtoul(line, NULL, 2);
			
			if(line == NULL){
        			continue;
      			}
      			
			//printf("CPU Load-writing to RAM %d at %d \n", byte, address);
			cpu->ram[address++] = byte;
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

    // TODO: implement more ALU ops
    
    case ALU_DIV:
      if(regB == 0){
      		fprintf(stderr, "You are trying to divide a number by 0, not permitted");
      } 
      else{
	      cpu->reg[regA] = cpu->reg[regA]/cpu->reg[regB];
      }	      
      break;
    
    case ALU_ADD:
      cpu->reg[regA] = cpu->reg[regA] + cpu->reg[regB];
      break;

    case ALU_SUB:
      cpu->reg[regA] = cpu->reg[regA] - cpu->reg[regB];
      break;
    
    default:
        printf("Unrecognized instruction");
        exit(1);  

  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char PC = cpu->PC;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

	unsigned char IR = cpu_ram_read(cpu, PC);
	unsigned operandA = cpu_ram_read(cpu, (PC + 1));
	unsigned operandB = cpu_ram_read(cpu, (PC + 2));
        int shift = ((IR >> 6)) + 1;

	switch(IR){

		case HLT:
                running = 0;
                break;

		case LDI:
		cpu->reg[operandA] = operandB;
        	PC += shift;
		break;

      		case PRN:
        	printf("%d \n", cpu->reg[operandA]);
        	PC += shift;
        	break;
		
		case PUSH:
		printf("Pushing value %d to Stack \n", cpu->reg[operandA]);
		cpu_ram_write(cpu, --cpu->reg[SP], cpu->reg[operandA]);
	        PC += shift;	
		break;

		case POP:
		cpu->reg[operandA] = cpu_ram_read(cpu, cpu->reg[SP]++);
		PC += shift;
		printf("Popping value %d from Stack \n", cpu->reg[operandA]);

		break;


		case MUL:
      		alu(cpu, ALU_MUL, operandA, operandB);
      		PC += shift;
		break;

		case DIV:
		alu(cpu, ALU_DIV, operandA, operandB);
		PC+= shift;
		break;

    		case ADD:
      		alu(cpu, ALU_ADD, operandA, operandB);
      		PC += shift;
		break;

		case SUB:
                alu(cpu, ALU_SUB, operandA, operandB);
                PC += shift;
		break;
		
		default:
        	printf("Unknown instruction %02x: %02x\n", PC, IR);
        	exit(1);

	}	


  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
   printf("CPU Init \n");
   
   cpu->PC = 0x00;
   
   cpu->reg[SP] = 0xF4;   //setting SP to higer address

   memset(cpu->reg, 0, sizeof(cpu->reg));
   memset(cpu->ram, 0, sizeof(cpu->ram));

  // TODO: Zero registers and RAM
}
