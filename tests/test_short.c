/** \file
 * \brief short test
 */

#include <limits.h>

#include "tests.h"

void test_short(FILE *output)
{
    FILE *input = open_inputFile("test_short");
#define UNSET 0
#define test(expectedError, expectedResult) test_tryRead(input, output, short, short, "%d", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0);
    test(IE_OK, 43);
    test(IE_OK, 0x2B);
    test(IE_OK, SHRT_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_OK, -1);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}