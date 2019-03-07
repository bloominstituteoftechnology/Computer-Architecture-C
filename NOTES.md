# Computer Architecture

## Outline of Steps to Build CPU
1. Review Main (ls8.c)
2. Define cpu struct (cpu.h)
3. Init / Power on State / LS-8 is booted
   - Define PC (cleared to 0)
   - Define Registers
      R0-R6 (cleared to 0)
      R7 (set to 0xF4)
   - RAM (cleared to 0 -- using memset)
4. cpu_load()
   - hard coded for now
5. cpu_run
   - need to define cpu_ram_read(MAR) and cpu_ram_write(MDR)
   - Define IR
   - Define operand0
   - Define operand1
   - switch IR
      case LDI
      case PRN
      case HLT
      default()
6. Adding more ALU (math instructions), e.g.:
      add #define MUL 0b1010001 (cpu.h)
      add ALU_MUL in enum alu_op (cpu.c) (symbolic name for a number)
      add case ALU_MUL:     (to switch(op) in alu() in cpu.c)
        cpu->reg[regA] = valA * valB;           (currying?)
        break;
      add case MUL:         (to switch(IR) in cpu_run() in cpu.c)
        alu(cpu, ALU_MUL, operand0, operand1);
        cpu->PC +=3;
        break;
7. Accounting for whitespace/comments in code


*** First 2 bits of instructions state how many operands ***

## Computer Architecture: Objectives
  Deconstruct the components of the CPU
  Understand how the CPU components communicate with RAM
  Learn the system bus ??

## Computer Architecture: Basics
Transistors
Gates (made up of transistors)
Digital Logic (common operations performed by gates)
  AND
  OR
  NOT
  NAND
  NOR
  XOR
Complex Gate Structures
  ALU
    ADD, MUL, etc.
  CPU
RAM
  Random Access Memory (RAM)
  Fast (compared to hard drives SSDs)
  Array of 8-bit numberss accessed by an index
  Each element in RAM stored as 1 byte (8-bit number)
  Larger, multi-byte values store in sequential addresses in RAM
  CPU communicates with RAM via the memory bus

Example:
Address 0   1   2   3   4   5   6   7
Value   07  A2  FF  7D  4E  F4  A3  8a

## CPU Terminolgy:
Bytes of data stored in RAM (memory)
Larger 64-bit (8-byte) numbers, stored sequentially in RAM that the CPU can operate on at once are called words
Exact number of bytes per work depends on the architucture:
  - 8 bytes for a 64-bit CPU
  - 4 bytes for a 32-bit CPU
  - 1 byte for an 8-bit CPU

## CPU Registers
Registers store words that can be accessed at ultra-high-speed
 - Similar to RAM, except stored directly on the CPU -- so much faster
 - Limited number of registers (8, 16, or 32)
 - Fixed names (R0, R1, etc. or EAX, EBX, etc.)
 - Many CPUs can only perform math opersons on registers which must be loaded from RAM first (x86 family can perform math on registers quickly or RAM slowly).

 ## CPU Instructions
 - Stored on RAM
 - Represented by numbers
 - Name given to instructions are convenient for devs, not computers
 - CPU tracks the address of currently-executing instruction in RAM & performs an action based on the current instruction
 - Program Counter (PC): stores the address of the currently-executing instruction


## CPU Stack
Stack data is stored in RAM
Stack pointer keeps track of the address of the top of the stack
PUSH and POP to add and remove items from the stack.
Typically, the stack starts at a higher memory address and grows down; the program starts at a low memory address and grows upwards.

PUSH
*stkptr decriments to a new address and the value is pushed into the stack at that address

POP
Capture value at the address the *stkptr is pointing to. Insert that value into R0 and *stkptr increments up to the next address. POP next value at R1. 
The value still remains at the address (usually it is not reset to zero, instead the next time a value is pushed in it just replaces the older value.)

When is the stack used?
Temporary storage of variables (ran out of registers for a temp value doing alu calcs)
Return address from a subroutine (subroutines are like functions)
Storage of registers and CPU state while handling an interrupt
Allocation of local variables for a subroutine

PUSH too many items on to a stack? Stack overload since the stack would have met in the middle with the program value and instructions.
POP from an empty stack? Grab an old value that had not been re-written over with a push?
How to detect if the stack is empty? Look to where the *stkptr is pointing to the top address.
During an interrupt, storage of registers and CPU state (i.e., the PC) must be stored on the stack

## Subroutines
Subroutines are similar to functions
CALL a subroutine at an address
RET (return) from that subroutine to where we left off
No arguments (just points to an address)
No return values
The return address of a subroutines stored is stored onto the stack
CALL pushes the address of the instruction *after it* onto the stack, then move the PC to the subroutine address.
RET will pop the return address of the stack and store it at the PC.

Example of a Subroutine

Parent routine
--------------
00: LDI R0, 15  (Load 15 (a value) onto register R0)
03: LDI R1, 0B  (Load 0B (an address) onto register R1)
06: CALL R1     (Push 08 (an address) onto stack then GOTO R1 (which has 0B))
08: PRN R0      (Print R0 current value is 25)
0A: HLT         (HLT)

Subroutine
--------------
0B: ADD R0, 10  (Adds 10 (a value) to R0)
0E: RET         (POP return address of stack (which is 08) and stores it at the PC)

Stack:  08  POP => empty
R0:     15  + 10 => 25
R1:     0B

Subroutines keep things dry

## Interupts
Handle interruption from a peripherals (keyboards, external storage)

Building Truth Tables from NAND
NAND  ~(A AND B)
NOT   A NAND A
AND   NOT(A NAND B)
OR    NAND(NOT A, NOT B)
NOR   NOT(OR)
XOR   OR(AND(NOT A, B) AND(A NOT B)))



### Half Adder
A and/or B in
Sum and/or Carry out

Half Adder Truth Table
Inputs      Outputs
A  B    Carry Out   Sum
0  0        0        0
0  1        0        1
1  0        0        1
1  1        1        0

C = AND(A, B)
S = XOR(A, B)


### Full Adder
A and/or B and/or Carry In
Sum and/or Carry out


Full Adder Truth Table
Inputs                  Outputs
A  B  Carry In    Carry Out   Sum
0  0      0           0        0
0  0      1           0        1
0  1      0           0        1
0  1      1           1        0
1  0      0           0        1
1  0      1           1        0
1  1      0           1        0
1  1      1           1        1

C = AND(A, B)
S = XOR(A, B)

CARRY-OUT = A AND B OR Cin(A XOR B) = A.B + Cin(A ⊕ B) = 
SUM = (A XOR B) XOR Cin = (A ⊕ B) ⊕ Cin = XOR(XOR(a, b), c)
Karnaugh Map

Full-Adder
Half-Adder with carry and the sum of the first

# Equivalent Equations
The 2nd equation replaces the no carry with a zero

  1   1 11      100010110
  010101001     010101001
+ 010001011   + 010001011
-----------   -----------
  100110100     100110100



Bit Shifting 
x = 0b10000000
y = x >> 6  // y is 0b00000010
1 is shifted to the right 6 times

NEED TO COMPLETE LATER
CPU
Transistors
Gates
Digital Logic
  AND OR NOT



CPU
Interrupt Handler (ISR/IMR)
PC IR
  Control 
  Registers
  ALU  


  Cache   
  RAM



Interrupt Handler


Keyboard    Disk    DMA

RAM (Random Access Memory)

1 byte (8 bit number)

Value 07 A2 FF

64-bit (8-byte) numbers

CPU Registers (R0 - R7)
Registers

CPU Instructions
CPU Clock 
  Clock Cycle Rate





 Dec to Binary to 
         ||||
 13 == 0b1101 == -3
 14 == 0b1110 == -2
 15 == 0b1111 == -1
 16 == 0b10000

 2's compliment
 IEEE-754 for floating point

 1’s complement of a binary number is another binary number obtained by toggling all bits in it, i.e., transforming the 0 bit to 1 and the 1 bit to 0.

Examples:

1's complement of "0111" is "1000"
1's complement of "1100" is  "0011" 
2’s complement of a binary number is 1 added to the 1’s complement of the binary number.
Examples:

2's complement of "0111" is  "1001"
2's complement of "1100" is  "0100" 

Binary Coded Decimal
26
0010 0110 BCD==26
1000 1001




