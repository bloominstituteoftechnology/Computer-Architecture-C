#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *filename)
{
  //open file and assign space for it 
  FILE *fp = fopen(filename, "r");
  char line[1024];
  unsigned char ram_address = 0x00; 

  // check for file 
  if (fp == NULL) {
    fprintf(stderr, "can't open file\n");
    exit(2);
  }

  while (fgets(line, sizeof(line), fp) != NULL) { // char *fgets(char *str, int n, FILE *stream)
    // man page - fgets() reads in at most one less than size characters from stream and stores them into the buffer pointed to by s. 
    char *end_ptr;
    unsigned char value = strtoul(line, &end_ptr, 2); //convert binary strings

    if (end_ptr == line) {
      // no numbers, so...
      continue;
    }
    // write ret to RAM
    cpu_ram_write(cpu,ram_address++, value);
  }

  fclose(fp);

}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char mar)
{
  return cpu->ram[mar];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr)
{
  cpu->ram[mar] = mdr;
}

/**
 * ALU
 */

void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL: //Multiply the values in two registers together and store the result in registerA.
      cpu->reg[regA] *= cpu->reg[regB];
      break;

    case ALU_ADD: //`ADD registerA registerB`
      cpu->reg[regA] += cpu->reg[regB];
      break;


    // ------- SPRINT ---------

    case ALU_CMP: //Compare the values in two registers.
      
      if (cpu->reg[regA] == cpu->reg[regB]) { //Equal `E` flag to 1, otherwise set it to 0.
        cpu->FL = cpu->FL | (1 << 0);
      }

      else if (cpu->reg[regA] > cpu->reg[regB]) { //Less-than `L` flag to 1,
        cpu->FL = cpu->FL | (1 << 1);
      }
  
      else {
        cpu->FL = cpu->FL | (1 << 2); //Greater-than `G` flag to 1, otherwise set it to 0.
      }

      break;

  }
}

/**
 * Run the CPU
 */

void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction 
  unsigned char IR; //store that result in `IR` ... This can just be a local
  unsigned char operandA;//into variables `operandA` and `operandB` in case the instruction needs them
  unsigned char operandB;
 

  while (running) {

    IR = cpu_ram_read(cpu, cpu->PC); // 1. Get the value of the current instruction (in address PC)./ IR
    
    operandA = cpu_ram_read(cpu, cpu->PC+1 & 0xff); // 3. Get the appropriate value(s) of the operands following this instruction
    operandB = cpu_ram_read(cpu, cpu->PC+2 & 0xff);



    int next_pc = (IR >> 6) + 1; // >> bitwise opperator, shifts right

    // printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, operandA, operandB);

    //for (int i = 0; i < 8; i++) {
     // printf(" %02X", cpu->reg[i]);
   // }

    //printf("\n");

    
    switch(IR) // 4. switch() over it to decide on a course of action. Cases - HTL / PRN / LDI /MUL
    {
      // 5. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu->reg[operandA] = operandB; 
        break;
      
      case PRN:
        printf("%d\n", cpu->reg[operandA & SP]);
        break;
      
      case MUL:  
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      
      case ADD:  
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      // ------- SPRINT ---------

      case CMP:  
        alu(cpu, ALU_CMP, operandA, operandB);
        break;

      case JMP:  
        // FILL ME IN
        break;

      case JEQ:  
        // FILL ME IN
        break;
      
      case JNE:  
        // FILL ME IN
        break;

      // ------- SPRINT END ---------
      case PUSH:
        cpu->reg[SP]--; //Decrement the `SP`.
        cpu->ram[cpu->reg[SP]] = cpu->reg[operandA]; //Copy the value in given register to the address pointed to by `SP`.
        break;
      
      case POP:
        cpu->reg[operandA] = cpu->ram[cpu->reg[SP]]; //Copy the value from the address pointed to by `SP` to the given register.
        cpu->reg[SP]++; //Increment `SP`.
        break;
      
      case CALL:
      /*Calls a subroutine (function) at the address stored in the register.

      1. The address of the ***instruction*** _directly after_ `CALL` is
        pushed onto the stack. This allows us to return to where we 
        left off when the subroutine finishes executing.
      2. The PC is set to the address stored in the given register. 
        We jump to that location in RAM and execute the first instruction in the subroutine.
        The PC can move forward or backwards from its current location.*/
       
        cpu->reg[SP]--;
        cpu->ram[cpu->reg[SP]] = cpu->PC + 2;
        cpu->PC = cpu->reg[operandA];
        break;
 
      case RET:
        cpu->PC = cpu->ram[cpu->reg[SP]];//Pop the value from the top of the stack and store it in the `PC`.
        cpu->reg[SP]++;
        break;

      case HLT:
        running = 0;
        break;
        
      default:
        printf("unexpected instruction 0x%02x at 0x%02x\n", IR, cpu->PC);
        exit(1);
    }
    // 6. Move the PC to the next instruction.
    //cpu->PC += next_pc;
    int set_pc = IR >> 4 & 0x01;
    if (set_pc == 0)
    {
      cpu->PC += next_pc;
    }


  }
}
   
/**
 * Initialize a CPU struct
 */

void cpu_init(struct cpu *cpu)
{
  // At first, the PC, registers, and RAM should be cleared to zero.
  cpu->PC = 0;
  cpu->reg[SP] = 0xF4; // The SP points at address `F4` if the stack is empty.
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  cpu->FL = 0;
    // man pages -  void *memset(void *s, int c, size_t n);
    // function fills the first n bytes of the memory area pointed to by s with the constant byte c.
}
