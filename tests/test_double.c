/** \file
 * \brief double test
 */

#include <math.h>

#include "tests.h"

void test_double(FILE *output)
{
    FILE *input = open_inputFile("test_double");
#define UNSET 0.0
#define test(expectedError, expectedResult) test_tryRead(input, output, double, double, "%g", floatComparer, UNSET, expectedError, expectedResult)

    test(IE_OK, 0.0);
    test(IE_OK, 0.75);
    test(IE_OK, 42.53);
    test(IE_OK, 43.0);
    test(IE_OK, -1.0);
    test(IE_OK, INFINITY);
    test(IE_OK, nan(""));
    test(IE_OK, 1.7e308);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);

    fclose(input);
}