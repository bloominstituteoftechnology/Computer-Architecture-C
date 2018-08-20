/ls8/cpu.h:
- Holds information about the CPU struct and declares the functions used by the CPU (load, init, run).
- Contains a define for LDI.

/ls8/cpu.c:
- includes cpu.h as a header
- cpu_load currently has a data array that is an instruction for LDI that loads the number 8 into register R0, an instruction for PRN that prints r0, and a HLT instruction, which exits the emulator, that it's passing into the RAM array on the cpu struct.
- void alu takes an instance of the cpu struct, an opcode, and two registers.
- cpu_run takes an instance of the cpu struct and has not yet been fleshed out.
- cpu_init has also not been fleshed out yet.

/ls8/ls8.c:
- contains main function that calls cpu_load, cpu_init, and cpu_run.

LS8-spec.md:
- States what registers are reserved and should be used for different internal registers
- Defines internal registers
- Describes flags
- Contains a memory map
- Describes in detail the process of an interrupt taking place
- Provides interrupt numbers
- Breaks down instruction bits
- Breaks down instructions

LS8-cheatsheet.md:
- Reference, but should double-check spec before using

Makefile:
- Takes all the .c files and assigns it to SRC
- Takes all the .h files and assigns it to HEADERS
- Takes all the .h and .c files and assigns them to DEPS
- ls8 is the target, DEPS are the dependencies.
- Runs gcc -Wall -Wextra -g -o with SRC as the last value
- .PHONY: clean tells the program that clean is a phony target which enables the use of 'make clean' though 'clean' is not a file name
- clean target then removes ls8.dSYM (the debug directories)

ls8/examples:
- Machine code instructions for different tasks.

asm/:
- Seems to be tests to verify that the programs are running as they should