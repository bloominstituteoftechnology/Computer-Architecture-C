Inventory for ls8:
* `cpu.c`: 
  * `cpu_ram_read/write`: read and write addresses to ram
  * `cpu_load`: loads the binary bytes from a .ls8 source file into a RAM array as its currently hard-coded
  * `alu`:
  * `cpu_run`: runs the CPU
    1. Get the value of the current instruction in address PC.
    2. switch() over it to decide on a course of action.
    3. Do the instruction should according to the spec.
    4. Move the PC to the next instruction.
  * `cpu_init`: initializes a CPU struct, the PC,registers

* `cpu.h`: Holds all information about the CPU.
  * holds information in CPU struct
  * `alu_up`:
  * `LDI` and other instructions will be defined here
* `ls8.c`: main() runs in this file.
  * runs `cpu_init`, `cpu_load`, `cpu_run`
* `Makefile`: 
  * assigns the .c files to SRC.
  * assigns the .h files to HEADERS.
  * assigns the SRC and HEADERS to DEPS.
  * runs gcc -Wall -Wextra -g -o with SRC as the last value.
* `/asm`: tests?  need to look into it.