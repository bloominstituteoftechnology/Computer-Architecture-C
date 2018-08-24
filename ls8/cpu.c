#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include "cpu.h"

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
// void cpu_load(struct cpu *cpu)
// {
//   const int DATA_LEN = 6;
//   char data[6] = {
//     // From print8.ls8
//     0b10000010, // LDI R0,8
//     0b00000000,
//     0b00001000,
//     0b01000111, // PRN R0
//     0b00000000,
//     0b00000001  // HLT
//   };

//   unsigned char address = 0;

//   for (int i = 0; i < DATA_LEN; i++) {
//     cpu->ram[address++] = data[i];
//   }

//   // TODO: Replace this with something less hard-coded
// }

void cpu_load(struct cpu *cpu, char *filename)
{
  FILE *fp;
  char line[1024];
  unsigned char address = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open file %s\n", filename);
    exit(2);
  }

  while (fgets(line, sizeof line, fp) != NULL) {

    char *end;
    unsigned char byte = strtoul(line, &end, 2);

    if (end == line) {
      continue;
    }

    cpu->ram[address++] = byte;
  }
}


// read RAM
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

// write RAM
unsigned char cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  return cpu->ram[address] = value;
}

//POP
unsigned char cpu_pop(struct cpu *cpu)
{
  unsigned char val = cpu_ram_read(cpu, cpu->registers[SP]);
  cpu->registers[SP]++;

  return val;
}

//PUSH
void cpu_push(struct cpu *cpu, unsigned char val)
{
  cpu->registers[SP]--;
  cpu_ram_write(cpu, cpu->registers[SP], val);
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
  unsigned char IR; // Instruction Register, contains a copy of the currently executing instruction
  unsigned char IR_size;
  unsigned char MAR; // Memory Address Register, holds the memory address we're reading or writing
  unsigned char MDR; // Memory Data Register, holds the value to write or the value just read

  unsigned char ch;

  while (running && ch != 'q') {
    // TODO
    //Stretch

    ch = io_bus_emulator();

    if (ch != NON_BREAKING_CODE) {
      printf("Key interrupt : %x\n",ch);
      cpu_ram_write(cpu,KEY_PRESSED,ch);
      //push to the stack to save cpu state
      cpu_push(cpu,cpu->PC);
      for(int i = 0; i <= SP-3 ;i ++)
        cpu_push(cpu,cpu->registers[i]);
      
      cpu->PC = cpu_ram_read(cpu, KEYBOARD_INTERRUPT_ADDRESS);
      printf("PC read from  KEYBOARD_INTERRUPT_ADDRESS : %d\n",cpu->PC);
    }

    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu,cpu->PC);
    // printf("Instrucction machine code %x PC %d\n",IR, cpu->PC);
    IR_size = (IR >> 6) + 1; // Get the instructions size ((IR >> 6) & 0x3) + 1;

    switch(IR_size) {
      case 3:
        MDR = cpu_ram_read(cpu, cpu->PC + 2);
      case 2:
        MAR = cpu_ram_read(cpu, cpu->PC + 1);
        break;
    }

    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    switch(IR) {
      case HLT:
        printf("HLT : %x\n",IR);
        running=0;
        break;

      case RET:
        printf("RET : %x\n",IR);
        cpu->PC = cpu_pop(cpu);
        printf("PC after read from SP : %d\n",cpu->PC);
        break;

      case IRET: //TODO
        printf("IRET : %x\n",IR);
        for(int i = SP - 3; i >= 0 ;i --)
          cpu->registers[i] = cpu_pop(cpu);
        cpu->PC = cpu_pop(cpu);
        printf("PC after read from SP : %d\n",cpu->PC);
        break;

      case LDI:
        printf("LDI : %x R%d %d\n",IR, MAR, MDR);
        cpu->registers[MAR] = MDR;
        break;

      case ST:
        printf("ST : %x R%d %d\n",IR, MAR, MDR);
        cpu_ram_write(cpu,cpu->registers[MAR],cpu->registers[MDR]);
        break;

      case LD:
        printf("LD : %x R%d %d\n",IR, MAR, MDR);
        cpu->registers[MAR] = cpu_ram_read(cpu, cpu->registers[MDR]);
        break;

      case CMP:
        printf("CMP : %x R%d R%d\n",IR, MAR, MDR);
        if(cpu->registers[MAR] == cpu->registers[MDR])
            cpu->FL = FLAG_EQUAL;
        else if(cpu->registers[MAR] < cpu->registers[MDR])
                cpu->FL = FLAG_LESS;
             else 
                cpu->FL = FLAG_GREATER;
        break;

      case PRN:
        printf("PRN : %x R%d\n",IR, MAR);
        printf("%d\n", cpu->registers[MAR]);
        break;

      case PRA:
        printf("PRA : %x R%d\n",IR, MAR);
        printf("%c\n", cpu->registers[MAR]);
        break;

      case MUL:
        printf("MUL : %x R%d %d\n",IR, MAR, MDR);
        alu(cpu,ALU_MUL,MAR, MDR);
        break;

      case ADD:
        printf("ADD : %x R%d %d\n",IR, MAR, MDR);
        alu(cpu,ALU_ADD,MAR, MDR);
        break;

      case PUSH:
        printf("PUSH : %x R%d\n",IR, MAR);
        cpu_push(cpu, cpu->registers[MAR]);
        break;

      case POP:
        printf("POP : %x R%d\n",IR, MAR);
        cpu->registers[MAR] = cpu_pop(cpu);
        break;

      case CALL:
        printf("CALL : %x R%d\n",IR, MAR);
        printf("PC before push : %d\n",cpu->PC);
        cpu_push(cpu,cpu->PC + 2);
        cpu->PC = cpu->registers[MAR];
        break;

      case JMP:
        printf("JMP : %x R%d\n",IR, MAR);
        printf("PC before jump : %d\n",cpu->PC);
        cpu->PC = cpu->registers[MAR];
        break;
      
      case JEQ:
        printf("JEQ : %x R%d\n",IR, MAR);
        if(cpu->FL == FLAG_EQUAL) {
          cpu->PC = cpu->registers[MAR];
        } else
          cpu->PC += 2;
        break;

      case JNE:
        printf("JNE : %x R%d\n",IR, MAR);
        if((cpu->FL & FLAG_EQUAL) == CLEAR_FLAG) {
          cpu->PC = cpu->registers[MAR];
        } else
          cpu->PC += 2;
        break;

      default:
        printf("instruction doesn't exist: %x\n",IR);
        // exit(2);
        break;
    }
    // 4. Move the PC to the next instruction.
    int instruction_set = (IR >> 4) & 1;

    if (!instruction_set)
    {
      cpu->PC += IR_size;
    }
    printf("PC : %d\n",cpu->PC);
    if (cpu->PC >= 100) // Just for test and prevent a infinte loop
      break;
    sleep(1);

  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Zero registers and RAM
  // Zero registers and RAM
  memset(cpu->registers, 0, sizeof cpu->registers);
  memset(cpu->ram, 0, sizeof cpu->ram);

  // TODO: Initialize the PC and other special registers
  cpu->PC = 0x00;
  cpu->FL = CLEAR_FLAG;
  cpu->registers[IM] = 0x00; // R5 is reserved as the interrupt mask (IM)
  cpu->registers[IS] = 0x00; // R6 is reserved as the interrupt status (IS)
  cpu->registers[SP] = EMPTY_STACK; // The SP points at the value at the top of the stack (most recently pushed), or at address F4 if the stack is empty.
}

// IO bus emulator: reads from keypress, without wait and echoes
int io_bus_emulator(void)
{
  struct timeval clock;
  gettimeofday(&clock, NULL);
  // printf("IO bus running: %lu\n", clock.tv_usec);
  struct termios oldattr, newattr;
  int ch;

  tcgetattr( STDIN_FILENO, &oldattr );
  newattr = oldattr;

  newattr.c_lflag &= ~( ICANON | ECHO); // clear keybuffer
  tcsetattr( STDIN_FILENO, TCSANOW, &newattr );

  system("stty -echo"); //kill echo

  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

  ch = getchar();
  // printf("Your pressed: %c\n",ch);

  system("stty echo"); //restore the echo
  tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
  return ch;
}