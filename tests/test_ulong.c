/** \file
 * \brief ulong test
 */

#include <limits.h>

#include "tests.h"

void test_ulong(FILE *output)
{
    FILE *input = open_inputFile("test_ulong");
#define UNSET 0ul
#define test(expectedError, expectedResult) test_tryRead(input, output, ulong, unsigned long, "%lu", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0ul);
    test(IE_OK, 43ul);
    test(IE_OK, 0x2Bul);
    test(IE_OK, ULONG_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}