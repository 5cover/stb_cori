/** \file
 * \brief Tests entry point
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define CORI_IMPLEMENTATION
#define CORI_DEFAULT_INPUT_ERROR_HANDLER
#include "tests.h"

#define OUT stdout

void handle_inputError(InputError error)
{
    assert(error && "error was IE_OK");
}

int main()
{
    puts("Testing started.");

    test_character(OUT);
    test_bool(OUT);
    test_float(OUT);
    test_double(OUT);
    test_longdouble(OUT);
    test_delimitedString(OUT);
    test_intmax(OUT);
    test_longlong(OUT);
    test_long(OUT);
    test_int(OUT);
    test_short(OUT);
    test_uintmax(OUT);
    test_ulonglong(OUT);
    test_ulong(OUT);
    test_uint(OUT);
    test_ushort(OUT);

    fprintf(OUT, "%zu tests (%zu succeeded, %zu failed)\n",
            g_testingResults.failureCount + g_testingResults.successCount,
            g_testingResults.successCount,
            g_testingResults.failureCount);

    return EXIT_SUCCESS;
}

/* Sample output

result name line:filename
(if failed)
Expected: expected
Actual: actualError

SUCCESS read_character_1 25:/.../read_character.c
FAILURE read_character_2 29:/.../read_character.c
    Expected 'IE_OK'
    Actual   'IE_MULTIPLE_CHARS'
SUCCESS ...

*/