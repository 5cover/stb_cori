/** \file
 * \brief long test
 */

#include <limits.h>

#include "tests.h"

void test_long(FILE *output)
{
    FILE *input = open_inputFile("test_long");
#define UNSET 0l
#define test(expectedError, expectedResult) test_tryRead(input, output, long, long, "%ld", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0l);
    test(IE_OK, 43l);
    test(IE_OK, 0x2Bl);
    test(IE_OK, LONG_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_OK, -1l);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}