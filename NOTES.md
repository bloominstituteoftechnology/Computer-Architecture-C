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

*** First 2 bits of instructions state how many operands ***

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


Full-Adder
Half-Adder with carry and the sum of the first

# Equivalent Equations
The 2nd equation replaces the no carry with a zero

  1   1 11      100010110
  010101001     010101001
+ 010001011   + 010001011
-----------   -----------
  100110100     100110100

  