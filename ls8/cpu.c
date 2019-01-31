#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DATA_LEN 1024

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index){
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char value){
  cpu->ram[index] = value;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file_path)
{
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };

  int data_length = 0;

  // TODO: Replace this with something less hard-coded
  FILE *fp;
  char line[1024];

  fp = fopen(file_path, "r");
  while(fgets(line, sizeof line, fp) != NULL){
    // printf("%s", line);
    if (line[0] == '#'){
      // printf("cpu_load: Leading #\n");
    }
    else{
      cpu->ram[data_length] = (unsigned char) strtoul(line, NULL, 2);
      // printf("ram %d - %x\n", data_length, cpu->ram[data_length]);
      data_length++;
    }
  }
  // printf("\n");
  fclose(fp);

}
// push value to top of stack
void cpu_push(struct cpu *cpu, unsigned char value){
  cpu->SP--;
  cpu->ram[cpu->SP] = value;
}
// pop the top of stack and return the value 
unsigned char cpu_pop(struct cpu *cpu){
  cpu->SP++;
  return cpu->ram[cpu->SP-1];
}

void cpu_jmp(struct cpu *cpu, unsigned char value){
  cpu->PC = value;
}

void cpu_jeq(struct cpu *cpu, unsigned char value){
  // printf("%d\n", (cpu->FL & 1));
  if ((cpu->FL & 1) == 1){
    cpu_jmp(cpu, value);
  }
  else{
    cpu_jmp(cpu, cpu->PC+2);
  }
}  

void cpu_jne(struct cpu *cpu, unsigned char value){
  // printf("%d\n", (cpu->FL & 1));
  if ((cpu->FL & 1) == 0){
    cpu_jmp(cpu, value);
  }
  else{
    cpu_jmp(cpu, cpu->PC+2);
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
      cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
      break;
    // TODO: implement more ALU ops
    case ALU_ADD:
      // printf("ADD: reg[operandA]: %d\n", cpu->registers[regA]);      
      // printf("     reg[operandB]: %d\n", cpu->registers[regB]);
      cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
      break;
    case ALU_CMP:
      if (cpu->registers[regA] == cpu->registers[regB]){
        cpu->FL = (cpu->FL | 0b00000001);
      }
      else if (cpu->registers[regA] > cpu->registers[regB]){
        cpu->FL = (cpu->FL | 0b00000010);
      }
      else {
        cpu->FL = (cpu->FL | 0b00000100);
      }
      break;
  }
}

void print_state(struct cpu *cpu){
  printf("PC: %x, IR: %x, SP: %x, R0: %x, R1: %x, R2: %x, stack-value: %x\n", cpu->PC, cpu->IR, cpu->SP, cpu->registers[0], cpu->registers[1], cpu->registers[2], cpu->ram[cpu->SP]);
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  unsigned char operandA;
  unsigned char operandB;
  unsigned char num_operand;
  while (running) {
    cpu->IR = cpu->ram[cpu->PC];
    // print_state(cpu);
    num_operand = cpu->IR >> 6;
    if (num_operand == 2){
        operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
        operandB = cpu_ram_read(cpu, (cpu->PC+2) & 0xff);
    }
    else if (num_operand == 1)
    {
        operandA = cpu_ram_read(cpu, (cpu->PC+1) & 0xff);
    }
    else{
    }
    
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
    switch(cpu->IR){
      //LDI: Set register[operandA] to operandB
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      //HLT: exit cpu_run
      case HLT:
        running = 0;
        break;
      //Add two operands and store value in registers[operandA]
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        break;
      //MUL: multiply two operands and store the product in resgister[operandA]
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        break;
      //CMP: compare the two operands and store the result in FL
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      //Push: store the value of operandA on top of stack
      case PUSH:        
        cpu_push(cpu, cpu->registers[operandA]);
        break;
      //POP: take the value at top of stack and store it in register[operandA]
      case POP:
        cpu->registers[operandA] = cpu_pop(cpu);
        break;
      //CALL: move PC to the address of operandA and push the address of the next instruction to top of stack
      case CALL:
        cpu_push(cpu, cpu->PC+2);
        cpu->PC = cpu->registers[operandA];
        // printf("Call SP: %x\n", SP);
        break;
      //RET: return PC from subroutine by moving PC to the address stored on top of stack
      case RET:
        cpu->PC = cpu_pop(cpu);        
        // printf("RET SP: %x\n", SP);
        break;
      //ST: store value of operandB in the address stored in operandA
      case ST:
        cpu->registers[operandA] = operandB;
        break;
      //JMP: jump PC to the address stored in operandA
      case JMP:
        cpu_jmp(cpu, cpu->registers[operandA]);
        break;
      //JEQ: If `equal` flag is set (true), jump to the address stored in the given register.
      case JEQ:
        cpu_jeq(cpu, cpu->registers[operandA]);
        break;
      //JNE: If `E` flag is clear (false, 0), jump to the address stored in the given register.
      case JNE:
        cpu_jne(cpu, cpu->registers[operandA]);
        break;
      default:
        break;
    }
    // if the instruction does not reset the PC value(as depicted by the 5th digit from the right of IR), move the PC to the next instruction by skipping the operands
    if ( ((cpu->IR >> 4) & 1) != 1){
      // printf("add operand\n");
      cpu->PC = cpu->PC + num_operand +1;
    }
  }
  exit(0);
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->SP = 0xF4;
  cpu->FL = 0;
  memset(cpu->registers, 0, sizeof(cpu->registers)); 
  memset(cpu->ram, 0, sizeof(cpu->ram)); 
  cpu->registers[7] = cpu->SP;
}