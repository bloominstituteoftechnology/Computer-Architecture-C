#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, unsigned char index, int data)
{
  cpu->ram[index] = data;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path)
{

  if (path == NULL)
  {
    path = "./examples/print8.ls8";
  }
  FILE *fileToRead = fopen(path, "r");
  if (fileToRead == NULL)
  {
    printf("error reading file");
  }
  char line[256];
  int address = 0;
  while (fgets(line, 256, fileToRead))
  {
    printf("first character of line: %c\n", line[0]);
    if (line[0] == '0' || line[0] == '1') {
    printf("line is: %s\n", line);
    cpu->ram[address] = strtoul(line, NULL, 2);
    printf("inserted into ram: %d\n", cpu->ram[address++]);
    } 
    //  else {
    // cpu->PC++;
    //  }
    }
  
 
}

/**
 * ALU
 */
//
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
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

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);
    unsigned char PRN;
    int result;
    int a;

    //<-- try putting int a over here
    // 2. switch() over it to decide on a course of action.
    printf("Performing instruction IR: %d\n", IR);
    switch (IR)
    {
    case 0b00000001: // HLT
      printf("halted");
      // what does running do? what should you do to running when you
      running = 0; // it shouldn't matter, but put a break under here. the compiler was complaining about it
      break;
      // lemme see what else
    case 0b10000010: // LDI? (Put comments on what the OPCODES are)

      a = cpu->R[cpu_ram_read(cpu, cpu->PC + 1)] = cpu_ram_read(cpu, cpu->PC + 2);

      printf("LDI: %d\n", a);
      cpu->PC += 3;
      break;
    case 0b01000111: // PRN Starts
      PRN = cpu->R[cpu_ram_read(cpu, cpu->PC + 1)];
      // the value you want to read from is in the register. However, `cpu_ram_read` is reading from the ram
      // the only thing you'll be reading from `cpu_ram_read` is the register to read from
      // but
      printf("%d\n", PRN);
      cpu->PC += 2;
      break;

    case 0b10100010: // MUL
      printf("MUL\n");            
      result = cpu->R[cpu_ram_read(cpu, cpu->PC + 1)] *= cpu->R[cpu_ram_read(cpu, cpu->PC + 2)];
      cpu->PC += 3;
      break;
    

    case 0b01000101: // PUSH
      cpu->SP--;
      cpu->ram[cpu->SP] = cpu->R[cpu_ram_read(cpu, cpu->PC +1)]; 
      cpu->PC += 2;
      printf("SP in PUSH %d\n", cpu->ram[cpu->SP]);
      break;

    case 0b01000110: // POP
      cpu->R[cpu_ram_read(cpu, cpu->PC +1)] = cpu->ram[cpu->SP]; 
      printf("SP in POP %d\n", cpu->ram[cpu->SP]);
      cpu->SP++;
      cpu->PC += 2;
      break;
    
    case 0b01010000: // CALL 
    //i'm still trying to think how i'm going to push the previous pc + 1 to the top of the stack so it can later pop it off and make pc that value 
    // the main thing i'm having trouble with is sec reading ls8-spec to compare what push needed to what push needs now if it's any different for the implementation
      cpu->ram[--cpu->SP] = cpu->PC + 2; // so the 
      // also, > The address of the ***instruction*** _directly after_ `CALL` is pushed onto the stack.
      // so basically if call was just a normal operation like LDI or ADD or something, where will the PC be. that's what gets pushed to the stack
      cpu->PC = cpu->R[cpu_ram_read(cpu, cpu->PC + 1)];
//there is a missing piece I need
//just cpu->PCi think so
        //might have went to far I think I push correctly line above on this line i'm trying to
      // this line i'm trying to eventually change the pointer to the right place but I think I need two lines to do it.
      //I don't really see why though. I want to just set the cpu->PC to the index that cpu->PC +2 should prove but I don't know if that messes stuff up
      break;
    case 0b00010001: //RET
      printf("PC IS POINTING AT\n");
      cpu->PC = cpu->ram[cpu->SP++];
      break;
    case 0b10100000: //ADD
      result = cpu->R[cpu_ram_read(cpu, cpu->PC + 1)] += cpu->R[cpu_ram_read(cpu, cpu->PC + 2)];
      cpu->PC = cpu->PC +3;
      break;
    default:
      printf("whatmeantho? %d\n", IR);
      break;
      //error: a label can only be part of a statement and a declaration is not a statement
      //running should be set to 0 to stop the loop once halted <-- :+1:
      //then I don't need the break but so that's the gist of it look for if the ram read is a certain code
      //if so do it? there's a lot of stuff in the spec. do we have to do this for pretty much each of them?
      // R: do it a little at a time. for example, the example program you need to run which is between lines 11-17
      // needs only three operations: LDI, PRN, and HLT
      // I think the readme asks you to make other programs work little by little, so add commands when you need them
      // and does it just kinda ignore the inputs that don't have commands attached? Lines 13,14 ,16 0b00001000,
      // don't have comments of commands associated with them so what are we doing with these values just checking
      // if they meet a condition and if not increase the PC counter and loop agin?
      // the answer is in the spec. look up LDI in LS8-spec.md
      // You'll see that the initial LDI
      // I get that LDI, PRN, and HLT will have conditions. I mean't unassigned commands. like lines 13,14, 16
      // that's what i mean. lines 13 and 14 are operands for the LDI command
      //ok i see
    }
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.
    // if (cpu->PC != 7)
    // {
    //   cpu->PC += 1;
    // }

    // <-  are you always going to move the PC by one all the time?
    // not if PC = 7 then we just wanna kill the switch case loop
    // sleep(1);
  }
}
/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM <-- O_O :eyes:
  for (int i = 0; i <= 7; i++)
  {
    if (i <= 6)
    {
      cpu->R[i] = 0;
    }
    else
    {
      cpu->R[i] = 0xF4;
      cpu->SP = cpu->R[i];
      printf("this thing worked stack pointer %d\n", cpu->SP);
    }
    //  }
  }
  cpu->PC = 0;
  cpu->FL = 0;

  for (int i = 0; i <= 255; i++)
  {
    cpu->ram[i] = 0;
  }
}