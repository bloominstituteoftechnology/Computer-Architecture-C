#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DATA_LEN 50

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char MAR )
{
  return cpu->ram[MAR];
}

void cpu_ram_write(struct cpu *cpu,unsigned char MAR, unsigned char MDR)
{
  cpu->ram[MAR] = MDR;
}

void cpu_load(char *filename, struct cpu *cpu)
{
  int RAM_address = 0; // initalizes address counter to 0 to be used as index for where values will be stored within RAM
  FILE *fd; // initializes file descriptor pointer to fd
  char line[1024]; // temp variable to hold each line of the file being read

  fd = fopen(filename, "r"); // open filename specified in argv and read file.

  if (fd == NULL) {
    printf("Cannot read file."); //print error if file cannot be read
    return;
  }

  while (fgets(line, sizeof line, fd) != NULL) { // while loop to loop through file and assign each line of file to RAM while loop exits once fgets returns NULL indicating EOF
    char *end_of_byte; //initialize pointer to end of byte
    unsigned char data = strtol(line, &end_of_byte, 2); // method used to only pull in a byte of information, and assign it to data.

    if (data == *line) { //if data and *line pointer equal one another continue; otherwise first iteration of fgets loads incorrect data.
      continue;
    }
    else {
      cpu->ram[RAM_address++] = data; // loads each line of data to a new RAM address location for cpu to read from when performing instructions.
    }
  }
  
  fclose(fd); //close file.
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

void cpu_run(struct cpu *cpu)
{
  unsigned char IR, operandA, operandB; // initializes IR, OpA, and OpB as "bytes"
  int instruction_index; // used to track location within RAM corresponding to sequential instructions.
  int running = 1; // True until we get a HLT instruction or if instruction fails to match switch case

  instruction_index = cpu->pc; // initialize index of instruction

  while (running) {
    IR = cpu_ram_read(cpu, instruction_index); // set IR to current value of instruction index
    operandA = cpu_ram_read(cpu, instruction_index + 1); // sets operandA to the next line below instruction
    operandB = cpu_ram_read(cpu, instruction_index + 2); // sets operandB to the second line below instruction, will only be used if needed in switch statement

     switch(IR||call) { // switch based on IR's instruction

      case LDI: //Set the value of a register to an integer.
        cpu->registers[operandA] = operandB; // save value in targeted register
        instruction_index += 3; // increments instruction index to next instruction line
        break; // breaks switch and re-runs loops

      case PRN: // print value of integer saved to register address
        printf("%d\n", cpu->registers[operandA]); // print value in targeted register
        instruction_index += 2; // increments instruction index to next instruction line
        break; // breaks switch and re-runs loops

      case MUL:
        cpu->registers[operandA] = cpu->registers[operandA]*cpu->registers[operandB];
        instruction_index += 3; // increments instruction index to next instruction line
        break;

      case PUSH:
        if(cpu->registers[7] == cpu->registers[0xF4]) {
          cpu->registers[7] = cpu->registers[operandA];
        } else {
          cpu->registers[7]--;
          cpu->registers[7] = cpu->registers[operandA];
        }
        instruction_index += 2; // increments instruction index to next instruction line
        break;

      case POP:
        if(cpu->registers[7] == cpu->registers[0xF4]) {
          printf("Stack empty nothing to pop");
          break;
        } else {
          cpu->registers[7] = cpu->registers[operandA];
          cpu->registers[7]++;
        }
        instruction_index += 2; // increments instruction index to next instruction line
        break;
          
      case HLT:
        running = 0; // kill while loop
          break;

      case CALL:
        unsigned char call:
        call = cpu->registers[operandA]
        instruction_index += 2; // increments instruction index to next instruction line

      default:
       printf("Nothing to run \n");
       running = 0; // kill while loop
     } 
  }
}

void cpu_init(struct cpu *cpu)
{
  cpu->pc = 0; //sets register to 0
  memset(cpu->registers, 0, sizeof cpu->registers); // uses memset to set register to 0
  memset(cpu->ram, 0, sizeof cpu->ram); // uses memset to set ram to 0
}
