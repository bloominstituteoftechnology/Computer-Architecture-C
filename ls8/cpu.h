#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  unsigned char PC;
  // registers (array)
  unsigned char registers[10];
  // ram (array)
  unsigned char ram[10];
};

// ALU operations
enum alu_op {
	ALU_MUL // what is this???
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

extern void cpu_load(struct cpu *cpu){

}; // what is this??
extern void cpu_init(struct cpu *cpu){
    // The `cpu_init()` function takes a pointer to
    //  a `struct cpu` and initializes it                                                         as necessary.At first,
    // the PC, registers, and RAM should be cleared 
    // to zero.(`memset()` might help you clear 
    // registers and RAM.)
    for (int i = 0; i < sizeof(cpu->ram); i++) {
     void memset(cpu, '\0', sizeof(cpu) );
    }
      
    };
extern void cpu_run(struct cpu *cpu){

};

#endif
