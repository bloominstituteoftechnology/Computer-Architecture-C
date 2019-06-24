#include <stdio.h>
#include <stdlib.h>

#define PRINT_BEEJ     1 
#define HALT           2
#define PRINT_NUM      3
#define PRINT_SUM      4
#define SAVE           5
#define PRINT_REGISTER 6
#define ADD            7
#define PUSH           8
#define POP            9

unsigned char registers[8]; // variables, reg 7 is the stack pointer

unsigned char memory[128];

int main(int argc, char **argv)
{
    int cur_index = 0;
    int running = 1;

    if (argc != 2) {
        fprintf(stderr, "usage: metaphor3 filename\n");
        return 1;
    }

    // Load the file
    char line[128];

    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "metaphor3: error opening file \"%s\"\n", argv[1]);
        return 2;
    }

    registers[7] = 128; // init to top of stack

    int address = 0;

    while (fgets(line, sizeof line, fp) != NULL) {
        char *endptr;

        unsigned char val = strtoul(line, &endptr, 10);

        if (endptr == line) {
            // Ignore lines with no numbers
            continue;
        }

        memory[address++] = val;
    }

    fclose(fp);

    // Run the machine as per usual

    unsigned char reg_num, val;
    unsigned char reg_num0, reg_num1;

    while (running) {
        unsigned char command = memory[cur_index];

				
		    printf("TRACE: reg_num: %d: val: %02X    operand0: %02x operand1: %02x\n", reg_num, val, reg_num0, reg_num1);

        switch (command) {
            case PUSH:
                registers[7]--;

                reg_num = memory[cur_index + 1];
                val = registers[reg_num];
                memory[registers[7]] = val;

                cur_index += 2;
                break;

            case POP:
                reg_num = memory[cur_index + 1];
                registers[reg_num] = memory[registers[7]];

                registers[7]++;

                cur_index += 2;
                break;

            case ADD:
                reg_num0 = memory[cur_index + 1];
                reg_num1 = memory[cur_index + 2];

                registers[reg_num0] += registers[reg_num1];

                cur_index += 3;
                break;

            case SAVE:
                val = memory[cur_index + 1];
                reg_num = memory[cur_index + 2];

                registers[reg_num] = val;

                cur_index += 3;
                break;

            case PRINT_REGISTER:
                printf("%d\n", registers[memory[cur_index + 1]]);

                cur_index += 2;
                break;

            case PRINT_BEEJ:
                printf("Beej!\n");
                cur_index++;
                break;

            case PRINT_NUM:
                printf("%d\n", memory[cur_index + 1]);
                cur_index += 2;
                break;
            
            case PRINT_SUM:
                printf("%d\n",
                    memory[cur_index + 1] +
                    memory[cur_index + 2]);
                cur_index += 3;
                break;
            
            case HALT:
                running = 0;
                cur_index++;
                break;

            default:
                printf("Unrecognized instruction\n");
                exit(1);
                break;
        }
    }

    return 0;
}