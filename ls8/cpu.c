#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

//fing cpu run

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

//fing cpu write

// void cpu_ram_write(struct cpu *cpu, unsigned char address)
// {
//   // cpu->ram[address] 
// }

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

//zoom ended
void cpu_load(struct cpu *cpu, char *argv){
  char* c;
  char buff[255];
  char buffer[255];
  FILE *fp = fopen(argv, "r");
  int address = 0;
  while(1) {
      c = fgets(buff, 9, (FILE*) fp);
      if( feof(fp) ) { 
         break ;
      }
      if((buff[0] == '0' || buff[0] == '1') && (buff[1] == '0' || buff[1] == '1') ){
        printf("new line :%s\n", c);
        printf("li :%li\n", strtoul(c, NULL, 2));
        cpu->ram[address++] = strtoul(c, NULL, 2);
      }
   }
   fclose(fp);
  
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };


  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded
}
//boooooooooo
/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op) {
//     case ALU_MUL:
//       // TODO
//       break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR;
  unsigned char val;
  int addrLDI;
  int addrPRN;

//what iz life?
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    // printf("cpu->PC is: %i\n", cpu->PC);
    IR = cpu_ram_read(cpu, cpu->PC);
    // printf("IR is: %d\n", IR);
    // IR 8 bits AA B C DDDD (AABCDDDD)
    switch (IR) {
      case 0b10000010: // LDI
        addrLDI = cpu_ram_read(cpu, cpu->PC + 1);
        val = cpu_ram_read(cpu, cpu->PC + 2);
        // printf("val is: %i\n, addrLDI is: %i\n", val, addrLDI);
        cpu->registers[addrLDI] = val;
        // printf("cpu->registers[addrLDI] is: %i\n", cpu->registers[addrLDI]);
        cpu->PC += 3;
        break;
      case 0b01000111: // PRN
        addrPRN = cpu_ram_read(cpu, cpu->PC + 1);
        // printf("addrPRN is: %i\n", addrPRN);
        printf("%i\n", cpu->registers[addrPRN]);
        cpu->PC += 2;
        break;
      case 0b00000001: //HLT
        running = 0;
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
  cpu->PC = 0;
  // TODO: Zero registers and RAM
  cpu->registers = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
}
