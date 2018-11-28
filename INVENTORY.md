# Inventory of LS-8

## cpu.h:
- Declares function used by CPU and holds information about CPU Struct.
- LDI define

## cpu.c:
- data array that tells LDI to load 8 into R0, which PRN prints r0, and HLT exits the emulator that passes into RAM array.
- void alu takes cpu strcut, opcode, and two registers.

## ls8.c:
- contains main function that calls cpu_load, cpu_init, and cpu_run.

## Makefile:
- .c files > SRC
- .h files > HEADERS
- .c and .h files > DEPS
- Runs gcc -Wall -Wextra -g -o with SRC as the last value
