<!-- Answers to the Short Answer Essay Questions go here -->
  
1. Explain how the CPU provides concurrency or parallelism:
Answer: The CPU can achieve concurrency or parallelism by performing  multiple things at once in various ways like  having multiple cores instead of just one core, and other features like pipelining.

In addition, the CPU can execute one program for a while (few ms), and then move to running another program for a while.

2. Describe assembly language and machine language:
Machine Language consists of binary digits that is 0s and 1s. 

Assembly language is the intermediate language between high level languages and machine language. In assembly language there is a strong correspondence between the program's code and the machine code instructions since it's a low level language.  

Assembly language statements have four sections. They are a label, mnemonic, operand, comment. Label and comments are optional. Mnemonic is theinstruction to execute and operands are parameters for the command. Assembly language also supports macros. A macro can be defined as a set of instructions with a name. It can be used elsewhere in the program.

source1-https://www.differencebetween.com/difference-between-machine-language-and-vs-assembly-language/
source2-https://en.wikipedia.org/wiki/Assembly_language

* Convert `11001111` binary

    to hex: 1100   = convert to deciaml = 8*1+4*1  = decimal value to hex (12)  = C

            1111    = 8+4+2+1 = 15 = F

    to hex 11001111 = CF


    to decimal: 128*1+64*1+32*0+16*0+8*1+4*1+2*1+1 = 207


* Convert `4C` hex

    to binary: 4 = 0100   C = 12 = 8+4 = 1100
    01001100

    to decimal: 4*16 + 1*C = 64 + 1*12 = 64+12 = 76


* Convert `68` decimal

    to binary: 64+4 = 128*0+64*1+32*0+16*0+8*0+4*1+2*0+1*0 = 01000100

    to hex: 68 = to decimal = 01000100 = divide into 4 digits each =  0100(4) 0100(4)  = 44
