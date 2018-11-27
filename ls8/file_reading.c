/*https://www.seas.upenn.edu/~cit593/cit593f09/lectures/fileIO.pdf
https://www.geeksforgeeks.org/fgets-gets-c-language/
https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm 

fopen(const char *name, const *char mode)
name is the name of the file
mode is how the file will be used r, w, a, rb
returns a pointer of type file

fgets()It reads a line from the specified 
stream and stores it into the string pointed 
to by str. It stops when either (n-1) characters 
are read, the newline character is read, or 
the end-of-file is reached, whichever comes first.

char *fgets(char *str, int n, FILE *stream)
str : Pointer to an array of chars where the string read is copied.
n : Maximum number of characters to be copied into str 
*stream : Pointer to a FILE object that identifies an input stream.
stdin can be used as argument to read from the standard input.
returns : the function returns str

strtol()

long int strtol(const char *str, char **endptr, int base)
str − This is the string containing the representation of an integral number.
endptr − This is the reference to an object of type char*, whose value is set by the function to the next character in str after the numerical value.
base − This is the base, which must be between 2 and 36 inclusive, or be the special value 0.
returns converted integral number as a long int value
 */

#include <stdio.h>   
#include <stdbool.h>

void read_file(int argc, char * argv[])
{
FILE *file_name;

if ((file_name = fopen(argv[0], "r")) == NULL)
    {
        printf("Cannot open file ...\n");
    }
fclose(file_name);
}