#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
// The type for a single unsigned byte in C is:
struct cpu {
  // DAY 1 -- TODO: Step 1
  // PC (program counter)
  unsigned int PC;
  // registers (array)
  unsigned char reg[8]; 
    //holds the data that CPU is currently processing | PC / IR / MAR / MDR
    //we'll execute code that stores the value 8 in a register
  // ram (array)
  unsigned char ram[256]; 
    //holds program instruction and data that the program requires for execution 
    //The LS-8 has 8-bit addressing, so can address 256 bytes of RAM total.
}; 

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define HLT  0b00000001
#define PRN  0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations
//DAY 1 - Add RAM functions cpu_ram_read(step 4 a few more details offered) and cpu_ram_write

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);
extern void cpu_ram_read(struct cpu *cpu);
extern void cpu_ram_write(struct cpu *cpu);

#endif
