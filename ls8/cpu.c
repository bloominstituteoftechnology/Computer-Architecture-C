#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define POP 0b01000110
#define PUSH 0b01000101


unsigned char cpu_ram_read(struct cpu *cpu, int mar) {
  return cpu->ram[mar];
}
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char mar, unsigned char mdr) {
  cpu->ram[mar] = mdr;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp;
  char data[1024];
  int address = 0;

  if( (fp = fopen(argv[1], "r") ) == NULL ) {

    printf("Error...\nCannot open file: %s\n", argv[1]);
    return -1;

  }
  
  while (fgets(data, sizeof data, fp) != NULL) {

    // Convert string to a number
    char *endchar;
    unsigned char byte = strtol(data, &endchar, 2);

    // Ignore lines from whicn no numbers were read
    if (endchar == data) {
      continue;
    }
    cpu->ram[address++] = byte;
  }

  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // unsigned char *registers = cpu->registers;

  // unsigned char valB = registers[regB];
  // switch (op) {
  //   case ALU_MUL:
  //     registers[regA] *= valB;
  //     break;
  //   case ALU_ADD:
  //     registers[regA] += valB;
  //     breakl
  //   // TODO: implement more ALU ops
  // }
}

void cpu_push(struct cpu *cpu, unsigned char val) {
  cpu->registers[7]--;
  cpu->ram[cpu->registers[7]] = val;
}

unsigned char cpu_pop(struct cpu *cpu) {
  unsigned char value = cpu->ram[cpu->registers[7]];
  cpu->registers[7]++;
  return value;
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  
  while (running) {
    
    // TODO
    // 1. Get the value of the current instruction (in address PC). 
    unsigned char ir = cpu_ram_read(cpu, cpu->PC); 
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);

    unsigned char opcode = ir;

    // printf("%d\n", opcode);
    
    // 2. switch() over it to decide on a course of action.
    switch(opcode) {
      case HLT:
        running = 0;
        break;

      case LDI:
        cpu->registers[operandA] = operandB;
        cpu->PC += 3;
        printf("LDI %d\n", cpu->registers[operandA]);
        break; 

      case PRN:
        printf("PRN %d\n", cpu->registers[operandA]);
        cpu->PC += 2;        
        break;
      
      // case ADD:
      //   alu(cpu, ALU_ADD, operandA, operandB);

      case PUSH:
        printf("PUSH %d\n", cpu->registers[operandA]);
        cpu_push(cpu, cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      case POP:
        cpu->registers[operandA] = cpu_pop(cpu);
        printf("POP %d\n", cpu->registers[operandA]);
        cpu->PC += 2;
        break;

      default:
        printf("PC %02x: unknown instruction %02x\n", cpu->PC, ir);
        exit(3);
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.  
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  
  // TODO: Zero registers and RAM

  cpu->PC = 0;
  memset(cpu->registers, 0, 8*sizeof(char));
  memset(cpu->ram, 0, 256*sizeof(char));

  return 0;
}
