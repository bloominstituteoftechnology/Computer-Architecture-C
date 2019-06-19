#include <stdio.h>
#include <stdlib.h>
// stdlib.h add this for strtoul()
// Takeaways: argc and argv give you command line info
// strtoul() "String to Unsigned Long" converts a number in a string into a numeric value that you can 
// store in a variable
// strtoul() accepts a base as the third parameter. E.g. binary base 2. 
// strtoul() is nice enough to skip over any leading whitespace before a number
// strtoul() once it starts parsing a number, it stops as soon as it finds a character that's not a valid
// digit in the given base

// strtoul() takes a pointer to a char * as the second argument. This can be used to detect invalid characters,
// as well as lines that don't contain any parseable numbers, we're using that second case. 

// If after the strtoul() call, the char * passed as the second argument is equal to the pointer to the line 
// passed in as the first argument, then no numbers were found. 

int main (int argc, char *argv[])
{
  FILE *fp;
	char line[1024];

  // checks for exactly 2 arguments
  if (argc != 2) {
		printf("usage: fileio filename\n");
		return 1;
	}

	// opens the file
	fp = fopen(argv[1], "r");

	// checks for file existing
	if (fp == NULL) {
		printf("Error opening file %s\n", argv[1]);
		return 2;
	}

	while (fgets(line, 1024, fp) != NULL) {

		char *endptr;

		unsigned char value = strtoul(line, &endptr, 2);

		// Check if no numbers were found:
		if (line == endptr) {
			printf("SKIPPING: %s", line);
			continue;
		}

		printf("%02x\n", value);

		// This is where you'd be store the value in memory in ls8's `cpu_load()`. 
	}

  fclose(fp);

}