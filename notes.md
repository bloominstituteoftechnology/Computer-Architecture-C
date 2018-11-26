ls8 is the c code 

asm has what looks to be a node script that converts asm to something that can run (.ls8 file)

ls8/examples - asm files for running on cpu.c

ls8/ls8.c - seems to be what sets the main loop for tests? (init,load,run) using a struct

ls8/cpu.h - struct needs to be defined, operations added, binary literals (?define), needs more instructions added

ls8/cpu.c - contains cpu_load, alu,cpu_run, and cpu_init in various states of progress 


