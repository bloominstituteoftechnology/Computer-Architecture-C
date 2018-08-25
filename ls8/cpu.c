#include "cpu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define DEBUG 0

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(char *filename, struct cpu *cpu)
{

  // Original code that I used for print8:
  // const int DATA_LEN = 6;
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8  /* instruction */
  //   0b00000000, /* argument 1 */
  //   0b00001000, /* argument 2 */
  //   0b01000111, // PRN R0 /* instruction */
  //   0b00000000, /* argument 1 */
  //   0b00000001  // HLT /* instruction */
  // };

  // int address = 0;

  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *fp; // file pointer
  char line[256]; // storage for line of file
  int counter = 0; // way to track index in ram

  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\r\n", filename);
    exit(1);
  }
    #if DEBUG
     printf("\n**********Lines from file:***********\r\n");
     #endif

    while (fgets(line, sizeof(line), fp) != NULL) { // read line from file and store in line up to 256 bytes.
      char *ptr;
      unsigned char byte = strtoul(line, &ptr, 2);
      if (ptr == line) {
        continue;
      }
      cpu->ram[counter++] = byte; // converts string to unsigned long integer using base 2 and stores in ram
      
      #if DEBUG
      printf("Value of line: %s\r", line);
      #endif
    }
   
    #if DEBUG
    printf("\n*******RAM in Load*******\r\n");
    for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\r\n", i, cpu->ram[i]);
    }
    printf("\r\n******end of RAM in Load*********\n");
    #endif
  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, int index){
  return cpu->ram[index];
}
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

int keypress_input(void)
{
  struct termios terminal_settings, new_terminal_settings;
  int ch;

  tcgetattr(STDIN_FILENO, &terminal_settings);
  new_terminal_settings = terminal_settings;

  new_terminal_settings.c_lflag &= ~(ICANON | ECHO);
  // Turns off ICANON and ECHO
  // ICANON is the flag for canonical, which is line by line reading
  // ... setting it instead to byte by byte
  // ECHO causes each key you type to be printed to the terminal
  tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_settings);
  // sets immediate changes to terminal

  system("stty -echo");
  // disable echo

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
  // manipulate file descriptor to set the NONBLOCK flag

  if(ch = getchar() != EOF) {
    printf("%c\n", ch);
  }
  system("stty echo");
  // enable echo
  tcsetattr(STDIN_FILENO, TCSANOW, &terminal_settings);
  // return terminal settings to what they were before this business
    return ch;
    // return character typed
  }
/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->reg;
  unsigned char valB = reg[regB];
  switch (op) {
    case ALU_ADD:
      reg[regA] += valB;
      break;
    case ALU_AND:
      reg[regA] = reg[regA] & reg[regB];
      break;
    case ALU_CMP:
      #if DEBUG
      printf("FL is %d\r\n", cpu->FL);
      #endif
      if ((regA - regB) < 0){
        cpu->FL = 4;
      }
      else if ((regA - regB) == 0){
        cpu->FL = 1;
      }
      else if ((regA - regB) > 0){
        cpu->FL = 2;
      }
      else {
        cpu->FL = 0;
      }
      #if DEBUG
      printf("FL is %d\n", cpu->FL);
      #endif
      break;
    case ALU_DEC:
      reg[regA] = reg[regA] - 1;
      break;
    case ALU_DIV:
      reg[regA] = reg[regA] / reg[regB];
      break;
    case ALU_INC:
      reg[regA] = reg[regA] + 1;
      break;
    case ALU_MOD:
      reg[regA] = reg[regA] % reg[regB];
      break;
    case ALU_MUL:
      reg[regA] *= valB;
      break;
    case ALU_NOT:
      reg[regA] = ~reg[regA];
      break;
    case ALU_OR:
      reg[regA] = reg[regA] | reg[regA];
      break;
    case ALU_SHL:
      reg[regA] = reg[regA] << reg[regB];
      break;
    case ALU_SHR:
      reg[regA] = reg[regA] >> reg[regB];
      break;
    case ALU_SUB:
      reg[regA] = reg[regA] - reg[regB];
      break;
    case ALU_XOR:
      reg[regA] = reg[regA] ^ reg[regB];
      break;
    // TODO: implement more ALU ops
  }
}

// void push(struct cpu *cpu, unsigned char regA)
// {
//   cpu_ram_write(cpu, --cpu->reg[7], regA);
// }

// unsigned char pop(struct cpu *cpu)
// {
//   unsigned char popped = cpu_ram_read(cpu, cpu->reg[SP++])
//   return popped;
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  unsigned char *reg = cpu->reg;
  // unsigned char PC = cpu->PC;
  int running = 1; // True until we get a HLT instruction
  unsigned char ch;

  while (running && ch != 'q') {

    ch = keypress_input();
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    int difference = ((IR >> 6) & 0b11) + 1; // shifts the number 6 places to the right (leaving last two places)
    // since the number of operands can be found in the two high bits, add one for opcode to get to next instruction
    unsigned char operandA = cpu_ram_read(cpu, cpu->PC+1);
    unsigned char operandB = cpu_ram_read(cpu, cpu->PC+2);
    unsigned char maskedInterrupts = cpu->reg[IM] & cpu->reg[IS];
    
      for (int i = 0; i < 8; i++) {
        if (((maskedInterrupts >> i) & 1) == 1){
          cpu->reg[IM] = 0;
          cpu->reg[IS] &= ~cpu->reg[IM];
          cpu_ram_write(cpu, --cpu->reg[SP], cpu->PC);
          cpu_ram_write(cpu, --cpu->reg[SP], cpu->FL);
          for (int r = 0; r < 7; r++) {
            cpu_ram_write(cpu, --cpu->reg[SP], cpu->reg[r]);
          }
          cpu->PC = cpu->ram[(0xF8 + i)]; // Because interrupt vector starts at F8
          break;
        }
      }
    switch(IR)
    {
      case ADD:
        alu(cpu, ALU_ADD, operandA, operandB);
        cpu->PC+=difference;
        break;
      case AND:
        alu(cpu, ALU_AND, operandA, operandB);
        cpu->PC+=difference;
        break;
      case CALL:
        #if DEBUG
        printf("Value before call: %d\r\n", cpu->PC);
        #endif
        cpu_ram_write(cpu, --cpu->reg[SP], cpu->PC+=difference);
        #if DEBUG
        printf("Value of PC in register: %d\r\n", cpu->PC);
        #endif
        cpu->PC = cpu->reg[operandA];
        #if DEBUG
        printf("Value of PC after call: %d\r\n", cpu->PC);
        #endif
        break;
      case CMP:
        alu(cpu, ALU_CMP, cpu->reg[operandA], cpu->reg[operandB]);
        cpu->PC+=difference;
        break;
      case DEC:
        alu(cpu, ALU_DEC, operandA, 0);
        cpu->PC+=difference;
        break;
      case DIV:
        if (reg[operandB] == 0) {
          fprintf(stderr, "DIV: Value in second register is 0");
          running = 0;
        } else {
          alu(cpu, ALU_DIV, operandA, operandB);
          cpu->PC+=difference;
        }
        break;
      case HLT:
        running = 0;
        break;
      case INC:
        alu(cpu, ALU_INC, operandA, 0);
        cpu->PC+=difference;
        break;
      case IRET:
        cpu->ram[cpu->reg[6]] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->reg[5]] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->reg[4]] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->reg[3]] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->reg[2]] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->reg[1]] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->reg[0]] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->FL] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->PC] = cpu->ram[cpu->reg[SP]++];
        cpu->ram[cpu->reg[IM]] = 2;
        break;
      case JEQ:
        if ((cpu->FL & 0b1) == 1){
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case JGE:
        if ((cpu->FL & 0b10) == 2 || (cpu->FL & 0b1) == 1){
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case JGT:
        if ((cpu->FL & 0b10) == 2){
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case JLE:
        if ((cpu->FL & 0b100) == 4) {
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case JLT:
        if ((cpu->FL & 0b1) == 1 || (cpu->FL & 0b100) == 4){
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case JMP:
        cpu->PC = cpu->reg[operandA];
        break;
      case JNE:
        if ((cpu->FL & 0b1) == 0){
          cpu->PC = cpu->reg[operandA];
        } else {
          cpu->PC+=difference;
        }
        break;
      case LD:
        reg[operandA] = reg[operandB];
        cpu->PC+=difference;
        break;
      case LDI:
        reg[operandA] = operandB;
        cpu->PC+=difference;
        break;
      case MOD:
        if (reg[operandB] == 0) {
          running = 0;
        }
        else {
          alu(cpu, ALU_MOD, operandA, operandB);
        }
        cpu->PC+=difference;
        break;
      case MUL:
        alu(cpu, ALU_MUL, operandA, operandB);
        cpu->PC+=difference;
        break;
      case NOP:
        cpu->PC+=difference;
        break;
      case NOT:
        alu(cpu, ALU_NOT, operandA, 0);
        cpu->PC+=difference;
        break;
      case OR:
        alu(cpu, ALU_OR, operandA, operandB);
        cpu->PC+=difference;
        break;
      case POP:
        reg[operandA] = cpu->ram[cpu->reg[SP]++];
        cpu->PC+=difference;
        break;
      case PRA:
        printf("%c", cpu->ram[reg[operandA]]);
        cpu->PC+=difference;
        break;
      case PRN:
        printf("%d\n", cpu->reg[operandA]);
        cpu->PC+=difference;
        break;
      case PUSH:
        cpu_ram_write(cpu, --cpu->reg[SP], cpu->reg[operandA]);
        cpu->PC+=difference;
        break;
      case RET:
        #if DEBUG
        printf("Value of PC before RET: %d\n", cpu->PC);
        #endif
        cpu->PC = cpu->ram[cpu->reg[SP]]++;
        #if DEBUG
        printf("Value of PC after RET: %d\n", cpu->PC);
        #endif
        break;
      case SHL:
        alu(cpu, ALU_SHL, operandA, operandB);
        cpu->PC+=difference;
        break;
      case SHR:
        alu(cpu, ALU_SHR, operandA, operandB);
        cpu->PC+=difference;
        break;
      case ST:
        cpu_ram_write(cpu, cpu->reg[operandA], reg[operandB]);
        cpu->PC+= difference;
        break;
      case SUB:
        alu(cpu, ALU_SUB, operandA, operandB);
        cpu->PC+=difference;
        break;
      case XOR:
        alu(cpu, ALU_XOR, operandA, operandB);
        cpu->PC+=difference;
        break;
      default:
        printf("Unknown instruction at %x: %x\r\n", cpu->PC, IR);
        exit(2);
    }

    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
  }
  #if DEBUG
    printf("\r\n********RAM in run********\r\n");
      for (unsigned long i = 0; i < 256; i++) {
      printf("cpu->ram[%lu] = %u\r\n", i, cpu->ram[i]);
    }
    printf("\r\n*******End of debugger********\r\n\r\n");
  #endif
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  cpu->reg = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
  cpu->reg[SP] = 0xF4;
  cpu->reg[IM] = 0x00;
  cpu->reg[IS] = 0x00;
  cpu->FL = 0x00;
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM
}
