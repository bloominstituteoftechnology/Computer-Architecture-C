# Computer Architecture

## Project

* [Implement the LS-8 Emulator](ls8/)

## Task List: add this to the first comment of your Pull Request

### Day 1: Get `print8.ls8` running
- [looked around] Inventory what is here
- [X] Implement `struct cpu` in `cpu.h`
- [X] Add RAM functions
- [X] Implement `cpu_init()`
- [X] Implement the core of `cpu_run()`
- [X] Implement the `HLT` instruction handler
- [X] Add the `LDI` instruction
- [X] Add the `PRN` instruction

### Day 2: Get `mult.ls8` running, load files dynamically
-> [X] Un-hardcode the machine code
- [X] Implement a Multiply and Print the Result (run `mult8.ls8`)
- [X] Beautify your `cpu_run()` function, if needed

### Day 3: Get `stack.ls8` running
- [X] Implement System Stack

### Day 4: Get `call.ls8` running
- [X] Implement Subroutine Calls

### Stretch
- [ ] Add the timer interrupt to the LS-8 emulator
- [ ] Add the keyboard interrupt to the LS-8 emulator
