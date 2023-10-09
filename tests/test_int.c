/** \file
 * \brief int test
 */

#include <limits.h>

#include "tests.h"

void test_int(FILE *output)
{
    FILE *input = open_inputFile("test_int");
#define UNSET 0
#define test(expectedError, expectedResult) test_tryRead(input, output, int, int, "%d", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0);
    test(IE_OK, 43);
    test(IE_OK, 0x2B);
    test(IE_OK, INT_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_OK, -1);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}