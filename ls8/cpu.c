#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

// helper functions

// read RAM
unsigned char cpu_ram_read(struct cpu *cpu, int address)
{
  return cpu->ram[address];
}

// write RAM
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  return cpu->ram[address] = value;
}

// push
void push(struct cpu *cpu, unsigned char value)
{
  cpu->registers[7]--;
  cpu->ram[cpu->registers[7]] = value;
}

// pop
void pop(struct cpu *cpu, unsigned char address)
{
  cpu->registers[address] = cpu->ram[cpu->registers[7]];
  cpu->registers[7]++;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  unsigned char address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  // TODO: Replace this with something less hard-coded

  unsigned char line[1024];   // set buffer or line of PC with size of 1024

  FILE *fp;  // need stdio.h

  fp = fopen(filename, "r");  // r = open the file for reading

  if (fp == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  while(fgets(line, sizeof line, fp) != NULL)
  {
    // printf("test %s\n", line);
    // or puts(line);
    unsigned char *end;   // end of binary number, cut it before #
    unsigned char b_number; // return the binary part

    b_number = strtoul(line, &end, 2);
    // printf("strtoul is %x\n", b_number);

    // if the line is empty which mean the beginning of the line has the same pointer (end), continue to the next line. AKA skip the blank line.
    if (line == end)
    {
      continue;
    }
    // load it in the memory
    cpu_ram_write(cpu, address++, b_number);
  }
}


/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      cpu->registers[regA] *= cpu->registers[regB];
      break;

    // TODO: implement more ALU ops

    case ALU_ADD:
      // TODO
      cpu->registers[regA] += cpu->registers[regB];
      break;
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  int address;
  unsigned char IR;   // IR: Instruction Register, contains a copy of the currently executing instruction
  int IR_size;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  
    address = cpu->PC;
    IR = cpu_ram_read(cpu, address);
    IR_size = (IR >> 6) + 1;  
    // printf("IR_Size %x\n", IR_size);
    unsigned char argv_a = cpu_ram_read(cpu, address + 1);
    unsigned char argv_b = cpu_ram_read(cpu, address + 2);

    switch (IR)
    {
      case LDI: 
        // printf("LDI : %x R%d %d\n",IR, argv_a, argv_b);
        cpu->registers[argv_a] = argv_b;
        // cpu->PC += 3;  // hard code before the bitwise setup `cpu->PC = IR_size;`
        break;
      
      case PRN:
        printf("%d\n", cpu->registers[argv_a]);
        // cpu->PC += 2;
        break;

      case HLT:
        running = 0;
        // cpu->PC += 1;
        break;

      case MUL:
        alu(cpu, ALU_MUL, argv_a, argv_b);
        break;
      
      case ADD:
        alu(cpu, ALU_ADD, argv_a, argv_b);
        break;

      case PUSH:
        // decrement the stack pointer
        cpu->registers[7]--;
        // then assign the value to the spot that sp points to
        cpu->ram[cpu->registers[7]] = cpu->registers[argv_a];
        // Vlad's approach which is pretty neat
        // write the value into the cpu->registers[7]
        // cpu_ram_write(cpu, cpu->registers[7], cpu->registers[argv_a]);
        break;

      case POP:
        // pretty much oposite from PUSH
        cpu->registers[argv_a] = cpu->ram[cpu->registers[7]];
        // Vlad's approch
        // cpu->registers[argv_a] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case CALL:
        cpu->registers[7]--;
        cpu_ram_write(cpu, cpu->registers[7], cpu->PC + (IR_size - 1));
        cpu->PC = cpu->registers[argv_a] - IR_size;
        break;

      case RET:
        cpu->PC = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case ST:
        // store value in b in the address stored in a
        cpu_ram_write(cpu, cpu->registers[argv_a], cpu->registers[argv_b]);
        break;

      case CMP:
        printf("CMP: IR %x, a %d, b%d", IR, argv_a, argv_b);
        if (cpu->registers[argv_a] == cpu->registers[argv_b])
          cpu->flag = 0x01;   // 0b00000001 (0b00000LGE) 
        else if (cpu->registers[argv_a] > cpu->registers[argv_b])
          cpu->flag = 0x02;   // 0b00000010
        else
          cpu->flag = 0x04;   // 0b00000100
        break;
          

      default:
        printf("error, invalid instruction %x\n", IR);
        break;
    }

    cpu->PC += IR_size; 
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;

  // initialize SP.  R7 reserves for SP starts at address F4 in RAM    
  cpu->registers[7] = 0xF4;  

  // init flag
  cpu->flag = 0;


  // TODO: Zero registers and RAM

  for (int i = 0; i < 256; i++) {   // 256 is the size of RAM
    cpu->ram[i] = 0;
  }

  for (int i = 0; i < 8; i++) {   // 8 register
    cpu->registers[i] = 0;
  }
  // for (int i = 0; i < 256; i++) {
  //   printf("%d\n", cpu->ram[i]);
  // }
}
