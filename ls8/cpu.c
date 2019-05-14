#include "cpu.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */

void cpu_load(struct cpu *cpu, char *path)
{ 

  // Check File Extension
  char *ext = &path[strlen(path) - 4];
  if (strcmp(ext, ".ls8") != 0){
    printf("%s is not a valid input. Input must be a .ls8 source file.\n", path);
    exit(1);
  }

  // Open File Path
  FILE *src;
  int lines = 0;
  src = fopen(path,"r");

  // Fail Check
  if (src == NULL) {
    printf("File %s could not be opened.\n", path);
    exit(2);
  } 

  // Count Number of Lines
  for (char c = getc(src); c != EOF; c = getc(src)) {
    if (c == '\n') {
      lines += 1;
    }
  }

  // Reset File Stream
  fseek(src, 0L, SEEK_SET);

  // Variables for Reading File
  char data[lines + 1];
  char line[255];
  char *cut;
  int count = 0;

  // Read File - strtol converts the binary only
  while(fgets(line, sizeof(line), src) != NULL) {
      if (line[0] == '0' || line[0] == '1') { 
          data[count] = strtol(line, &cut, 2);
          count += 1;
      } else {
          continue; // Skip lines without instructions
      }
  }

  // Close File
  fclose(src);

  // Initialize Instructions to RAM
  for (int address = 0; address < count + 1; address++) {
    cpu->ram[address] = data[address];
  }
}

// Read From CPU Ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address) {
  return cpu->ram[address];
}

// Write To CPU Ram
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
    case ALU_ADD:
      cpu -> registers[regA] = cpu -> registers[regA] + cpu -> registers[regB];
      break;
    // TODO: implement more ALU ops
  }
}


/**
 * Handler Functions
 */

// Instruction Jump Table
void (*handlers[256])(struct cpu *cpu, unsigned char op0, unsigned char op1) = {0};

// General Instructions
void LDI_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { cpu->registers[op0] = op1; } // Set Register  
void HLT_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { (void)cpu; (void)op0; (void)op1; exit(0); } // Halt
void PRN_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { (void)op1; printf("%d\n", cpu->registers[op0]); } // Print

// ALU Instructions
void ADD_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { alu(cpu, ALU_ADD, op0, op1); } // Add
void MUL_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { alu(cpu, ALU_MUL, op0, op1); } // Multiply

// Stack Instructions
void POP_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { // Pop
  (void)op1; 
  cpu->registers[op0] = cpu_ram_read(cpu, cpu->registers[7]);
  if (cpu->registers[7] != 0xF4) cpu->registers[7] += 1; 
}

void PUSH_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { // Push
  (void)op1;
  if (cpu->registers[7] != 0x00) {
    cpu->registers[7] -= 1; 
    cpu_ram_write(cpu, cpu->registers[7],  cpu->registers[op0]); 
  } else {
    printf("Stack Overflow.");
    exit(4);
  }
}

// PC Explicit Instructions
void CALL_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { // Call
  (void)op1;
  cpu->registers[7] -= 1; 
  cpu_ram_write(cpu, cpu->registers[7], cpu->PC + 2); 
  cpu->PC = cpu->registers[op0];
}

void RET_handler (struct cpu *cpu, unsigned char op0, unsigned char op1) { // Return
  (void)op1; (void)op0;
  cpu->PC = cpu_ram_read(cpu, cpu->registers[7]);
  if (cpu->registers[7] != 0xF4) cpu->registers[7] += 1;
}


/**
 * Main Loop
 */

void cpu_run(struct cpu *cpu)
{

  // Assign Functions to Jump Table
  handlers[LDI] = LDI_handler;
  handlers[HLT] = HLT_handler;
  handlers[PRN] = PRN_handler;
  handlers[MUL] = MUL_handler;
  handlers[ADD] = ADD_handler;
  handlers[HLT] = HLT_handler;
  handlers[POP] = POP_handler;
  handlers[PUSH] = PUSH_handler;
  handlers[CALL] = CALL_handler;
  handlers[RET] = RET_handler;

  int running = 1; // True until we get a HLT instruction

  while (running) {

    // Registers For Debugging
    // printf("\n%d\n", cpu->registers[0]);
    // printf("%d\n", cpu->registers[1]);
    // printf("%d\n", cpu->registers[2]);
    // printf("%d\n", cpu->registers[3]);
    // printf("%d\n", cpu->registers[4]);
    // printf("%d\n", cpu->registers[5]);
    // printf("%d\n", cpu->registers[6]);
    // printf("%x\n", cpu->registers[7]);

    // Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // Figure out how many operands this next instruction requires
    unsigned char ops = (IR & 0xC0) >> 6;

    // Get the appropriate value(s) of the operands following this instruction
    unsigned char op0 = cpu_ram_read(cpu, cpu->PC + 1);
    unsigned char op1 = cpu_ram_read(cpu, cpu->PC + 2);

    // Do whatever the instruction should do according to the spec.
    if (handlers[IR]) { handlers[IR](cpu, op0, op1); }
    else { printf("Unkown Instruction: 0x%02x\n", IR); running = 0;}

    // Move the PC to the next instruction, if instruction is not explicit.
    if ((IR & 0x10) != 0x10) { cpu->PC += (ops + 1); } 
    
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
  cpu->registers[7] = 0xF4;
}
