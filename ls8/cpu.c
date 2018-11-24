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

    case ALU_ADD:
      cpu->reg[regA] += cpu->reg[regB];
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
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;
      
      case PRN:
        printf("The trace above PRN \n\n");
        printf("%d\n", cpu->reg[operandA]);
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;

      case MUL:
        printf("The trace above MUL \n\n");
        alu(cpu, ALU_MUL, operandA, operandB);
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;

      case ADD:
        printf("The trace above ADD \n\n");
        alu(cpu, ALU_ADD, operandA, operandB);
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;

      case PUSH:
        printf("The trace above PUSH \n\n");
        // Decrement the SP.
        // Copy the value in the given register to the address pointed to by SP.
        cpu_ram_write(cpu, --cpu->reg[7], cpu->reg[operandA]);
        printf("Read from RAM at SP: %02X\n", cpu_ram_read(cpu, cpu->reg[7]));
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;

      case POP:
        printf("The trace above POP \n\n");
        //Copy the value from the address pointed to by SP to the given register.
        //Increment SP.
        cpu->reg[operandA] = cpu_ram_read(cpu,cpu->reg[7]++);
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;

      case CALL:
        printf("The trace above CALL \n\n");
        //The address of the instruction directly after the CALL instruction is pushed onto the stack.
        cpu_ram_write(cpu, --cpu->reg[7] , ++cpu->PC);
        //The PC is set to the address stored in the given register.
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;

      case RET:
        printf("The trace above RET \n\n");
        //Pop the value from the top of the stack and store it in the PC.
        cpu->PC = cpu_ram_read(cpu,cpu->reg[7]++);
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;
      
      case CMP:
        printf("The trace above CMP \n\n");
        //Compare the values in RegA and RegB and set the appropriate flag
        if (cpu->reg[operandA] == cpu->reg[operandB]){
          cpu->flag_reg[0] = 1; // E = Equals = 0th bit
        }else if (cpu->reg[operandA] > cpu->reg[operandB]){
          cpu->flag_reg[1] = 1; // G = Greater than = 1st bit
        }else {
          cpu->flag_reg[2] = 1; // L = Less than = 2nd bit
        }
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;
      
      case JMP:
        printf("The trace above JMP \n\n");
        //Jump to the address stored in the given register.
        //Set the PC to the address stored in the given register.
        cpu->PC = cpu->reg[operandA];
        add_to_pc = 0;
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;
      
      case JEQ:
        printf("The trace above JEQ \n\n");
        //If equal flag is set (true), jump to the address stored in the given register.
        if (cpu->flag_reg[0] == 1){
          cpu->PC = cpu->reg[operandA];
          add_to_pc = 0;
        }
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;
      
      case JNE:
        printf("The trace above JNE \n\n");
        // If E flag is clear (false, 0), jump to the address stored in the given register.
        if (cpu->flag_reg[0] == 0){
          cpu->PC = cpu->reg[operandA];
          add_to_pc = 0;
        }
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
        break;
      
      case HLT:
        printf("The trace above HLT \n\n");
        printf("RAM value at F4: %02X\n", cpu_ram_read(cpu, 0xF4));
        printf("RAM value at F3: %02X\n", cpu_ram_read(cpu, 0xF3));
        printf("RAM value at F2: %02X\n", cpu_ram_read(cpu, 0xF2));
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

  // // TODO: Zero registers and RAM and Flag Registers
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->flag_reg, 0, sizeof(cpu->flag_reg));

  cpu->reg[7] = 0b11110100;
  
}


