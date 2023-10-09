/** \file
 * \brief float test
 */

#include <math.h>

#include "tests.h"

void test_float(FILE *output)
{
    FILE *input = open_inputFile("test_float");
#define UNSET 0.0f
#define test(expectedError, expectedResult) test_tryRead(input, output, float, float, "%g", floatComparer, UNSET, expectedError, expectedResult)

    test(IE_OK, 0.0f);
    test(IE_OK, 0.75f);
    test(IE_OK, 42.53f);
    test(IE_OK, 43.0f);
    test(IE_OK, -1.0f);
    test(IE_OK, INFINITY);
    test(IE_OK, nanf(""));
    test(IE_OK, 3.4e38f);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);

    fclose(input);
}