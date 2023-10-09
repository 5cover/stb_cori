#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// #define CORI_DEFINITION static inline
#define CORI_DEFAULT_INPUT_ERROR_HANDLER
#define CORI_IMPLEMENTATION
#include "stb_cori.h"

void handle_inputError(InputError error)
{
    char *s;
    switch (error) {
    case IE_EMPTY: s = "Empty input"; break;
    case IE_EOF: s = "EOF reached"; break;
    case IE_INVALID_BOOL_CHAR: s = "Invalid boolean char"; break;
    case IE_MULTIPLE_CHARS: s = "Multiple chars"; break;
    case IE_NOT_A_NUMBER: s = "Not a number"; break;
    case IE_NUMBER_OUT_OF_BOUNDS: s = "Number out of bounds"; break;
    case IE_OUT_OF_MEMORY: s = "Out of memory"; break;
    case IE_UNSUPPORTED_BASE: s = "Unsupported numeric base"; break;
    default: abort();
    }
    puts(s);
}

void handle_exampleInputError(InputError error)
{
    printf("Error: %d\n", error);
}

int main()
{
    InputError error;

    puts("Example: read");
    {
        int result;
        printf("Type an integer: ");
        result = read_int();
        printf("Recieved %d\n", result);
    }
    puts("Example: read_handleErrors");
    {
        printf("Type an integer (max %d): ", SHRT_MAX);
        short result = read_short_handleErrors(handle_exampleInputError);
        printf("Recieved %d\n", result);
    }
    puts("Example: tryRead");
    {
        int result;
        do {
            printf("Type an integer smaller than 10: ");
            error = tryRead_int(&result);
        } while (error || result >= 10);
        printf("Recieved %d\n", result);
    }
    puts("Example: read_from");
    {
        FILE *readme = fopen("README.md", "r");
        char *firstLine = read_line_from(readme);
        printf("First line of README: \"%s\"\n", firstLine);
        free(firstLine);
        fclose(readme);
    }
    puts("Example: read_bool");
    {
        char const yesChars[] = "yY", noChars[] = "nN";
        printf("Do you like this library? ");
        bool result = read_bool(yesChars, noChars);
        puts(result ? "Thank you" : "That's ok");
    }
}
