#include <stdio.h>

const int PRINT_BEEJ = 1;
const int PRINT_12 = 3;
const int SAVE = 4;
const int PRINT_SAVED = 7;
const int HALT = 2;

int memory[] = {
    PRINT_BEEJ,
    SAVE,
    2,
    128,
    PRINT_SAVED,
    2,
    SAVE,
    0,
    37,
    PRINT_SAVED,
    2,
    PRINT_SAVED,
    0,
    //ADD,
    //0,
    //2,
    HALT
};

int PC = 0;

int saved_value[8];

int main(void)
{
    int sv_index;

    for(;;) {
        int c = memory[PC];

        switch (c) {
            case PRINT_BEEJ:
                PC++;
                printf("Beej!\n");
                break;

            case PRINT_12:
                PC++;
                printf("12!\n");
                break;

            case SAVE:
                sv_index = memory[PC + 1];
                saved_value[sv_index] = memory[PC + 2];
                PC += 3;
                break;

            case PRINT_SAVED:
                sv_index = memory[PC + 1];
                printf("Saved: %d\n", saved_value[sv_index]);
                PC += 2;
                break;

            case HALT:
                PC++;
                return 0;
        }
    }

    return 0;
}