#include <stdlib.h>
#include <stdio.h>

#define PRINT_BEEJ 1
#define HALT 2
#define PRINT_NUM 3
#define PRINT_SUM 4
#define SAVE 5
#define PRINT_REGISTER 6
#define ADD 7

// The idea here is that a piece of software can be written to run another piece of software within that virtual machine.
// What does a virtual machine need to do?
// Print "Beej" on the screen and exit.
// Read instructions and values from memory, then run in a loop and excute those instructions

unsigned char registers[8]; // variables

unsigned char memory[128];

int main(int argc, char **argv)
{

  int pc = 0; // Program Counter (index of current instruction)
  int ir = 1; // Instruction Register (currently running instruction)

  // Load File
  FILE *fp;

  char line[128];

  // checks for exactly 2 arguments
  if (argc != 2)
  {
    printf("usage: fileio filename\n");
    return 1;
  }

  // opens the file
  fp = fopen(argv[1], "r");

  // checks for file existing
  if (fp == NULL)
  {
    printf("Error opening file %s\n", argv[1]);
    return 2;
  }

  int address = 0;

  while (fgets(line, 1024, fp) != NULL)
  {

    char *endptr;

    unsigned char value = strtoul(line, &endptr, 10);

    // Check if no numbers were found:
    if (line == endptr)
    {
      printf("SKIPPING: %s", line);
      continue;
    }

    printf("%02x\n", value);

    // This is where you'd be store the value in memory in ls8's `cpu_load()`.

    memory[address++] = value;
  }

  fclose(fp);

  // Run the machine per usual
  unsigned char reg_num, value;
  unsigned char reg_num0, reg_num1;

  while (ir)
  {

    unsigned char command = memory[pc];

    switch (command)
    {
    case ADD:
      reg_num0 = memory[pc + 1];
      reg_num1 = memory[pc + 2];

      registers[reg_num0] += registers[reg_num1];

      pc += 3;
      break;

    case SAVE:
      value = memory[pc + 1];
      reg_num = memory[pc + 2];

      registers[reg_num] = value;

      pc += 3;
      break;

    case PRINT_REGISTER:
      printf("%d\n", registers[memory[pc + 1]]);
      pc += 2;
      break;

    case PRINT_BEEJ:
      printf("BEEJ!\n");
      pc += 1;
      break;

    case PRINT_NUM:
      printf("%d\n", memory[pc + 1]);
      pc += 2;
      break;

    case PRINT_SUM:
      printf("%d\n", memory[pc + 1] + memory[pc + 2]);
      pc += 3;
      break;

    case HALT:
      ir = 0;
      pc += 1;
      break;

    default:
      printf("Unrecognized instruction\n");
      exit(1);
      break;
    }
  }
  return 0;
}