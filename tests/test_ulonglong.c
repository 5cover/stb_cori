/** \file
 * \brief ulonglong test
 */

#include <limits.h>

#include "tests.h"

void test_ulonglong(FILE *output)
{
    FILE *input = open_inputFile("test_ulonglong");
#define UNSET 0ull
#define test(expectedError, expectedResult) test_tryRead(input, output, ulonglong, unsigned long long, "%llu", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0ull);
    test(IE_OK, 43ull);
    test(IE_OK, 0x2Bull);
    test(IE_OK, ULLONG_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}