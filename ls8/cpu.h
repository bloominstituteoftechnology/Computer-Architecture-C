#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU

struct cpu {
  // TODO
  unsigned char PC; // this is good
  unsigned char FL; // this too i think For the array switch thing would I just have a 7 slot array then
  //lol I don't evven remember how to array in c XD
   // hold up what are you doing? O_O lol you said to do it as an array and then I wanted to iterate over each one to make them 0
   // in the struct, you declare an array. you ain't doin stuff to it in the struct definition. or are you trying to null everything?  yeah beccause this
   // which is still not in the struct definition. 
   // the struct simply defines the structure of whatever you're making. the actual manipulation of the values it stores does not happen here
  int R[8]; //0xF4
  unsigned char ram[256];  //256 stays the same but need to maybe havve it an array of structs or something still with 256 slots
  // R: yep you need something with 256 slots. right now, you just have a variable with the number 256 on it
};

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index) {

}

cpu_ram_write()

// ALU operations
enum alu_op {
	ALU_MUL
	// Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
