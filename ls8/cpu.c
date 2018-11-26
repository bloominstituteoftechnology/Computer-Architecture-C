#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_ADD:
      regA = regA + regB;
      break;
    case ALU_AND:
      if(regA && regB){
        regA = 1;
      } else {
        regA = 0;
      }
      break;
    case ALU_CALL:
      regA = regA + regB;
      break;
    //   case ADD:
    case ALU_CMP:
    
      break;
    case ALU_DEC:
    
      break;
    case ALU_DIV:
    
      break;
    case ALU_HLT:
    
      break;
    case ALU_INC:
    
      break;
    case ALU_IRET:
    
      break; 
    case ALU_JEQ:
    
      break;
    case ALU_JGE:
    
      break;
    case ALU_JGT:
    
      break;
    case ALU_JLE:
    
      break;
    case ALU_JLT:
    
      break;
    case ALU_JMP:
    
      break;
    case ALU_JNE:
    
      break;
    case ALU_LD:

      break;
    case ALU_LDI:
    
      break;
    case ALU_MOD:
    
      break;
    case ALU_MUL:
      // TODO
      regA = regA * regB;
      break;
    case ALU_NOP:
    
      break;
    case ALU_NOT:
    
      break;
    case ALU_OR:

      break;
    case ALU_POP:
    
      break;
    case ALU_PRA:
    
      break;
    case ALU_PRN:
    
      break;
    case ALU_PUSH:
    
      break; 
    case ALU_RET:
    
      break;
    case ALU_SHR:
    
      break;
    case ALU_ST:

      break;
    case ALU_SUB:
    
      break;
    case ALU_XOR:

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

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    switch(cpu->PC){
      case ADD:
        alu(cpu, ALU_ADD, cpu->PC+1, cpu->PC+2);
        cpu->PC = cpu->PC + 3;
        break;
      case AND:
        
        break;
      case CALL:
        
        break; 
      case CMP:
        
        break;
      case DEC:
        
        break;
      case DIV:
        
        break;
      case HLT:
        
        break;
      case INC:
        
        break;
      case IRET:
        
        break; 
      case JEQ:
        
        break;
      case JGE:
        
        break;
      case JGT:
        
        break;
      case JLE:
        
        break;
      case JLT:
        
        break;
      case JMP:
        
        break;
      case JNE:
        
        break;
      case LD:
  
        break;
      case LDI:
        
        break;
      case MOD:
        
        break;
      case MUL:
        
        break;
      case NOP:
        
        break;
      case NOT:
        
        break;
      case OR:
  
        break;
      case POP:
        
        break;
      case PRA:
        
        break;
      case PRN:
        
        break;
      case PUSH:
        
        break; 
      case RET:
        
        break;
      case SHR:
        
        break;
      case ST:
  
        break;
      case SUB:
        
        break;
      case XOR:
  
        break;

    }
    // 2. switch() over it to decide on a course of action.
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
  cpu->ram = memset();
  cpu->registers = memset();
  cpu->PC = memset();
  // TODO: Zero registers and RAM
}
