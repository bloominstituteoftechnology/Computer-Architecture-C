#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h> // for timeval (timed interrupts)

#define DATA_LEN 255
#define STACK_POINTER 7  // Define the register reserved for the stack pointer
#define IM 5 // Interrupt mask
#define IS 6 // Interrupt status



// Declare an array of pointers to functions, add functions to array in cpu_init
int (*instructions[DATA_LEN]) (struct cpu * cpu, unsigned char regA, unsigned char regB) = {0};

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char* fileName)
{
  char data[DATA_LEN];

  FILE* file_ptr;

  if((file_ptr = fopen(fileName, "r")) == NULL)
  {
    printf("Error opening file!\nPlease include a relative path with filename\n");
    exit(1);
  }

  int i = 0;
  while(fgets(data, sizeof(data), file_ptr) != NULL)
  {
    char * endptr;

    unsigned char num;
    num = strtoul(data, &endptr, 2);

    if(endptr == data)
    {
      continue;
    }

    cpu_ram_write(cpu, i, num);     // Write binary instructions to ram

    i++;
  }
  
  fclose(file_ptr);
}

/**
 * ALU
 */
int alu(struct cpu *cpu, unsigned char op, unsigned char regA, unsigned char regB)
{
  switch (op) 
  {
      case MUL:
      {
        unsigned char product = cpu->registers[regA] * cpu->registers[regB];
        cpu->registers[regA] = product;
        return 1;
        break;
      }
      case ADD:
      {
        unsigned char sum = cpu->registers[regA] + cpu->registers[regB];
        cpu->registers[regA] = sum;
        return 1;
        break;
      }
        
      default:
        return 1;

  }
}


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{

  int running = 1; // True until we get a HLT instruction

  struct timeval tv;  
  int last_sec = -1;

  while (running) {

    gettimeofday(&tv, NULL);
    
    if (tv.tv_sec != last_sec) {
      cpu->registers[IS] |= 0b00000001; // Set first bit of Interrupt Status to 1;
      last_sec = tv.tv_sec;
    }
    
    handle_interrupt(cpu);


    // Get the value of the current instruction (in address PC).
    int PC = cpu->PC;
    unsigned char binary_instruction = cpu_ram_read(cpu, PC);
    unsigned char operandA = cpu_ram_read(cpu, PC + 1);
    unsigned char operandB = cpu_ram_read(cpu, PC + 2);
    
    // Set and check our instruction handler

    int (*handler)(struct cpu*, unsigned char, unsigned char);

    handler = instructions[binary_instruction];

    if(handler == 0)
    {

      printf("Invalid instruction PC: %02X entered, exiting program.\n", PC);
      break;

    }

    // Do whatever the instruction should do according to the spec.
    
    if(((binary_instruction >> 4) & 0x1) != 1)  // If we have an instruction which DOES NOT change the PC (indicated by bit 000x0000)
    {
      
      running = handler(cpu, operandA, operandB);   // Returns 1 if continue, 0 if halt
      cpu->PC += (int) (binary_instruction >> 6) + 1;

    }
    else// If we have an instruction which DOES change the PC
    {

      cpu->PC = handler(cpu, operandA, operandB);   // Returns the PC position to jump to

    }

  }
}


// Instruction Handlers
unsigned char cpu_ram_read(struct cpu *cpu, int mar)
{

  return cpu->ram[mar]; //mar = memory address register

}

void cpu_ram_write(struct cpu *cpu, int mar, unsigned char value)
{

  cpu->ram[mar] = value;
  
}

int handle_LDI(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  cpu->registers[regA] = regB;
  return 1;
  
}

int handle_PRN(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  printf("\nValue at register %d is: %d\n", regA, cpu->registers[regA]);
  return 1;
  
}

int handle_MUL(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  alu(cpu, MUL, regA, regB);
  return 1;
  
}

int handle_ADD(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  alu(cpu, ADD, regA, regB);
  return 1;
  
}

int handle_PUSH(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->registers[regA]); // Decrement stack pointer first, then write value in register A to stack (according to spec)
  return 1;
  
}

int handle_POP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  unsigned int stack_val = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Get the value at stack pointer, then increment
  handle_LDI(cpu, regA, stack_val);               // Set the desired register = popped value
  return 1;

}

int handle_HALT(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)cpu;
  (void)regA;
  (void)regB;
  return 0;

}

int handle_CALL(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  // Push address of next instruction to the stack
  cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->PC + 2);
  return cpu->registers[regA];                                            // return the value in register A (the PC value of the called function)

}

int handle_RET(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regA;
  (void)regB;
  // Pop PC address of next instruction off of the stack
  unsigned int return_pc = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Get the value at stack pointer, then increment
  return return_pc;                                            // return the PC value of the address just after the previous call

}

int handle_ST(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // Store value in registerB in the address stored in registerA.
  cpu_ram_write(cpu, cpu->registers[regA], cpu->registers[regB]);
  return 1;

}

int handle_CMP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // Clear the current flags
  cpu->FL &= 0;

  // |= is or equals, sets any 1 to true
  if(cpu->registers[regA] < cpu->registers[regB])
  {
    cpu->FL |= 0b0000100;
  }
  else if(cpu->registers[regA] > cpu->registers[regB])
  {
    cpu->FL |= 0b0000010;
  }
  else if(cpu->registers[regA] == cpu->registers[regB])
  {
    cpu->FL |= 0b0000001;
  }

  return 1;

}

int handle_JMP(struct cpu* cpu, unsigned char regA, unsigned char regB)
{
  
  (void)regB;
  // Jump to the address stored in the given register
  return cpu->registers[regA];

}

int handle_JEQ(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // If equal flag is set (true), jump to the address stored in the given register
  if((cpu->FL & 1) == 1)
  {
    return handle_JMP(cpu, regA, regB);
  }
  // If not equal, increment PC by 2 and return value
  int move_increment = 2;
  return cpu->PC + move_increment;

}

int handle_JNE(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  // If E flag is clear (false, 0), jump to the address stored in the given register
  if((cpu->FL & 1) != 1)
  {
    return handle_JMP(cpu, regA, regB);
  }
  // If equal, increment PC by 2 and return value
  int move_increment = 2;
  return cpu->PC + move_increment;

}

int handle_PRA(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regB;
  printf("\nValue at register %d is: %c\n", regA, cpu->registers[regA]);
  return 1;

}

int handle_IRET(struct cpu* cpu, unsigned char regA, unsigned char regB)
{

  (void)regA;
  (void)regB;

  for(int i = 0; i < 7; i++)  // pop registers 0 through 6 off the stack
  {
    handle_POP(cpu, i, 0);
  }

  cpu->FL = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Pop off Flags
  cpu->PC = cpu_ram_read(cpu, cpu->registers[STACK_POINTER]++); // Pop off PC
  handle_POP(cpu, IM, 0); // Pop old IM off stack, re-enabling interrupts
  
  return cpu->PC;

}

// Check for interrupts and handle

void handle_interrupt(struct cpu* cpu)
{
    // Check if interrupts are enabled. Interrupt mask will be > 0
    if(cpu->registers[IM] > 0)
    {
      unsigned char maskedInterrupts = cpu->registers[IM] & cpu->registers[IS];

      for (int i = 0; i < 8; i++) {
        // Right shift interrupts down by i, then mask with 1 to see if that bit was set
        int interrupt_happened = ((maskedInterrupts >> i) & 1) == 1;

        if(interrupt_happened)
        {
          cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->registers[IM]); // push IM to stack
          cpu->registers[IM] &= 0;       // Set IM to 0, disabling further interrupts
          cpu->registers[IS] &= 0;      // Clear all interrupts
          cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->PC); // push PC to stack
          cpu_ram_write(cpu, --cpu->registers[STACK_POINTER], cpu->FL); // push FL to stack
          
          for(int i = 0; i < 7; i++)  // push registers 0 through 6 to the stack
          {
            handle_PUSH(cpu, i, 0);
          }
          
          cpu->PC = cpu->ram[0xF8 + i];
        }

        break;
      }
    }

}



/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // Place in ram where stack memory begins
  int BEGIN_STACK = 0xF4;

  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FL = 0;

  // Zero registers and RAM
  memset(cpu->ram, 0, sizeof cpu->ram);
  memset(cpu->registers, 0, sizeof cpu->registers);

  cpu->registers[STACK_POINTER] = BEGIN_STACK;

  // Assign our functions to the array of function handlers
  instructions[LDI] = handle_LDI;
  instructions[PRN] = handle_PRN;
  instructions[MUL] = handle_MUL;
  instructions[ADD] = handle_ADD;
  instructions[PUSH] = handle_PUSH;
  instructions[POP] = handle_POP;
  instructions[HLT] = handle_HALT;
  instructions[CALL] = handle_CALL;
  instructions[RET] = handle_RET;
  instructions[ST] = handle_ST;
  instructions[CMP] = handle_CMP;
  instructions[JMP] = handle_JMP;
  instructions[JEQ] = handle_JEQ;
  instructions[JNE] = handle_JNE;
  instructions[PRA] = handle_PRA;
  instructions[IRET] = handle_IRET;


}
