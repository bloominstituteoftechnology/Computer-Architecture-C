# Computer Architecture

## Project

* [Implement the LS-8 Emulator](ls8/)

## Task List: add this to the first comment of your Pull Request

### Day 1: Get `print8.ls8` running
- [ ] Inventory what is here
- [ ] Implement `struct cpu` in `cpu.h`
- [ ] Add RAM functions
- [ ] Implement the core of `cpu_run()`
- [ ] Implement the `HLT` instruction handler
- [ ] Add the `LDI` instruction
- [ ] Add the `PRN` instruction

List of files:
1. In the ls8 folder, you have cpu.c, cpu.h, ls8.c, and Makefile.
a. cpu.c -- It has 3 functions: cpu_load() which loads a .ls8 file that contains binary bytes into the RAM array;
            alu(); cpu_run() which runs the CPU; and cpu_init() which initializes a CPU struct.
b. cpu.h -- Has 1 function: cpu() which holds all information about the CPU.
c. ls8.c -- Has 1 function: main() which is the main function of the LambdaSchool-8 (LS-8) 8-bit 
            CPU/computer/emulator.


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
