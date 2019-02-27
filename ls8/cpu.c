#include "cpu.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path)
{ 
  char *ext = &path[strlen(path) - 4];
  if (strcmp(ext, ".ls8") != 0){
    printf("%s is not a valid input. Input must be a .ls8 source file.\n", path);
    exit(1);
  }

  FILE *src;
  int lines = 0;
  src = fopen(path,"r");

  if (src == NULL) {
    printf("File %s could not be opened.\n", path);
    exit(2);
  } 

  for (char c = getc(src); c != EOF; c = getc(src)) {
    if (c == '\n') {
      lines += 1;
    }
  }

  fseek(src, 0L, SEEK_SET);

  char data[lines + 1];
  char line[255];
  char *cut;
  int count = 0;

  while(fgets(line, sizeof(line), src) != NULL) {
      if (line[0] == '0' || line[0] == '1') { 
          data[count] = strtol(line, &cut, 2);
          count += 1;
      } else {
          continue;
      }
  }

  fclose(src);

  int address = 0;

  for (int i = 0; i < count + 1; i++) {
    cpu->ram[address++] = data[i];
  }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value) {
  cpu->ram[address] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{

  switch (op) {
    case ALU_MUL:
      cpu -> registers[regA] = cpu -> registers[regA] * cpu -> registers[regB];
      break;
    // TODO: implement more ALU ops
  }
}


/**
 * Handler Functions
 */

void (*handlers[256])(struct cpu *cpu, unsigned char op0, unsigned char op1) = {0};

void LDI_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { cpu->registers[op0] = op1; } // Set Register  
void PRN_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { (void)op1; printf("%d\n", cpu->registers[op0]); } // Print
void MUL_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { alu(cpu, ALU_MUL, op0, op1); } // Multiply
void HLT_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { (void)cpu; (void)op0; (void)op1; exit(0); } // Halt

void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {

    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // 2. Figure out how many operands this next instruction requires
    unsigned char ops = (IR & 0xC0) >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char op0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char op1 = cpu_ram_read(cpu, cpu->PC + 2);

    // 4. switch() over it to decide on a course of action.
    handlers[LDI] = LDI_handler;
    handlers[PRN] = PRN_handler;
    handlers[MUL] = MUL_handler;
    handlers[HLT] = HLT_handler;

    // 5. Do whatever the instruction should do according to the spec.
    if (handlers[IR]) { handlers[IR](cpu, op0, op1); }
    else { printf("Unkown Instruction: 0x%02x\n", IR); running = 0;}

    // 6. Move the PC to the next instruction.
    cpu->PC += (ops + 1);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->registers, 0, 8);
  memset(cpu->ram, 0, 256);
}
