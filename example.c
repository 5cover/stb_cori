#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #define CORI_DEFINITION static inline

#define CORI_IMPLEMENTATION
#include "stb_cori.h"

int main()
{
    InputError error;

    puts("Example: read_linel");
    {
        char answer[26];
        printf("Type the alphabet: ");
        read_linel(26, answer);
        printf("Recieved %s\n", answer);
    }
    puts("Example: read_int");
    {
        int result;
        printf("Type an integer: ");
        result = read_int();
        printf("Recieved %d\n", result);
    }
    puts("Example: read_short_or");
    {
        printf("Type an integer (max %d): ", SHRT_MAX);
        short result = read_short_or(cori_handle_error_stderr_msg);
        printf("Recieved %d\n", result);
    }
    puts("Example: tryRead_int");
    {
        int result;
        do {
            printf("Type an integer smaller than 10: ");
            error = tryRead_int(&result);
        } while (error || result >= 10);
        printf("Recieved %d\n", result);
    }
    puts("Example: read_line_from");
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
        printf("Do you like this library? (y/n) ");
        bool result = read_bool(yesChars, noChars);
        puts(result ? "Thank you" : "*Ernie prepares to commit a hate crime*");
    }
}
