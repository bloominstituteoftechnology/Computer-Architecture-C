#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATA_LEN 6

// read from memory mar = memory address register
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

// Write to memory
void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}


/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  //Init the file
  FILE *fp = fopen(filename, "r");
	
  //Init the line that hold 1024 characters
  char line[1024];

  //Init the Mem Address Register to 0
  unsigned char mar = 0x00;

  //Check if the file exits:
  if (fp == NULL){
    fprintf(stderr, "error opening file %s\n", filename);
    exit(2);
  }

  //While there's lines in the file...
	while (fgets(line, sizeof line, fp) != NULL) {
    char *endptr;
		unsigned char machine_code = strtoul(line, &endptr, 2);

    if (endptr == line){
      //we got no numbers
      continue;
    }

    //Write it into Memory
    cpu_ram_write(cpu, mar++, machine_code);
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
      cpu->reg[regA] *= cpu->reg[regB];
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

  unsigned char IR; //instruction register
 
  printf("\nCPU Running....\n");

  while (running) {
    // 1. Get the value of the current instruction (in address PC).

    IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xFF);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xFF);

    printf("HEADR: PC | OC OA OB | R0 R1 R2 R3 R4 R5 R6 R7\n");
    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    for(int i = 0; i < 8; i++) {
      printf(" %02X", cpu->reg[i]);
    }

    printf("\n");


    int add_to_pc = (IR >> 6) + 1;
      // 2. switch() over it to decide on a course of action.
    switch (IR) {
      case LDI:
        // LDI
        // 3. Do whatever the instruction should do according to the spec.
        printf("The trace above is LDI \n\n");
        cpu->reg[operandA] = operandB;
        break;
      
      case PRN:
        printf("The trace above PRN \n\n");
        printf("%d\n", cpu->reg[operandA]);
        break;

      case MUL:
        printf("The trace above MUL \n\n");
        alu(cpu, ALU_MUL, operandA, operandB);
        break;

      case HLT:
        printf("The trace above HLT \n\n");
        running = 0;
        break;
    }

    // 4. Move the PC to the next instruction.
    cpu->PC += add_to_pc;
  }


}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Power On State
  // R0-R6 are cleared to 0.
  // R7 is set to 0xF4.
 
  // PC and FL registers are cleared to 0.
  // RAM is cleared to 0.
  printf("\nCPU Init starting...\n");
  
  // Initialize the PC and other special registers
  cpu->PC = 0;

  // // TODO: Zero registers and RAM
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));

 cpu->reg[7] = 0b11110100;
}


