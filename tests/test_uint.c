/** \file
 * \brief uint test
 */

#include <limits.h>

#include "tests.h"

void test_uint(FILE *output)
{
    FILE *input = open_inputFile("test_uint");
#define UNSET 0u
#define test(expectedError, expectedResult) test_tryRead(input, output, uint, unsigned int, "%u", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0u);
    test(IE_OK, 43u);
    test(IE_OK, 0x2Bu);
    test(IE_OK, UINT_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}