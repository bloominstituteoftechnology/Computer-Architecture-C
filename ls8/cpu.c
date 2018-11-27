#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8 <- this means Line 12: LDI, 13: R0, 14: 8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0 <- 15: PRN
    0b00000000, // <- 16: R0
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded PRR
}

/**
 * ALU
 */
//
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
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
  
  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu->PC);

    // 2. switch() over it to decide on a course of action.
    switch(IR)
    {
      case 0b00000001: // HLT
        printf("halted");
        // what does running do? what should you do to running when you 
        running = 0;
      case 10000010: //LDI STARTS
        cpu->R[cpu_ram_read(cpu, cpu->PC+1) ] = cpu_ram_read(cpu, cpu->PC+2);
        break;
      case 01000111: // PRN Starts
        float PRN = cpu_ram_read(cpu, cpu->R[cpu_ram_read(cpu, cpu->PC+1)]);
        printf("%d\n", PRN);
        break;
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
    cpu->PC+= 1;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers

  // TODO: Zero registers and RAM <-- O_O :eyes:
 for (int i = 0; i <= 7; i++) {
   if (i <= 6) {
     cpu->R[i] = 0;
     } else {
     cpu->R[i] = 0xF4;
  }
  //  }
}
cpu->PC = 0;
cpu->FL = 0;

for (int i = 0; i <= 255; i++) {
  cpu->ram[i] = 0;
}
}