# Computer Architecture

## Project

* [Implement the LS-8 Emulator](ls8/)

## Task List: add this to the first comment of your Pull Request

### Day 1: Get `print8.ls8` running
- [ ] Inventory what is here
- [ ] Implement `struct cpu` in `cpu.h`
- [ ] Add RAM functions
- [ ] Implement `cpu_init()`
- [ ] Implement the core of `cpu_run()`
- [ ] Implement the `HLT` instruction handler
- [ ] Add the `LDI` instruction
- [ ] Add the `PRN` instruction

### Day 2: Get `mult.ls8` running, load files dynamically
- [ ] Un-hardcode the machine code
- [ ] Implement a Multiply and Print the Result (run `mult8.ls8`)
- [ ] Beautify your `cpu_run()` function, if needed

### Day 3: Get `stack.ls8` running
- [ ] Implement System Stack

### Day 4: Get `call.ls8` running
- [ ] Implement Subroutine Calls

### Stretch
- [ ] Add the timer interrupt to the LS-8 emulator
- [ ] Add the keyboard interrupt to the LS-8 emulator


#### File structure:

* `/examples`: Code instructions to be referenced when performing different tasks.
* `cpu.c`: 
  * `cpu_ram_read/write`: read and write addresses to ram
  * `cpu_load`: loads the binary bytes from a .ls8 source file into a RAM array (currently hard-coded)
  * `alu`:
  * `cpu_run`: runs the CPU
    1. Get the value of the current instruction (in address PC).
    2. switch() over it to decide on a course of action.
    3. Do whatever the instruction should do according to the spec.
    4. Move the PC to the next instruction.
  * `cpu_init`: initializes a CPU struct, the PC, and other special registers

* `cpu.h`: Holds all information about the CPU.
  * holds information in CPU struct
  * `alu_up`:
  * `LDI` and other instructions will be defined here
* `ls8.c`: main() runs here
  * runs `cpu_init`, `cpu_load`, `cpu_run`
* `Makefile`: 
  * assigns all the .c files to SRC
  * assigns all the .h files to HEADERS
  * assigns all the SRC and HEADERS to DEPS
  * runs gcc -Wall -Wextra -g -o with SRC as the last value
* `/asm`: looks like tests, could be some nuggets in there