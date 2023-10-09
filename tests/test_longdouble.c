/** \file
 * \brief longdouble test
 */

#include <math.h>

#include "tests.h"

void test_longdouble(FILE *output)
{
    FILE *input = open_inputFile("test_longdouble");
#define UNSET 0.0L
#define test(expectedError, expectedResult) test_tryRead(input, output, longdouble, long double, "%Lg", floatComparer, UNSET, expectedError, expectedResult)

    test(IE_OK, 0.0L);
    test(IE_OK, 0.75L);
    test(IE_OK, 42.53L);
    test(IE_OK, 43.0L);
    test(IE_OK, -1.0L);
    test(IE_OK, (long double)INFINITY);
    test(IE_OK, nanl(""));
    test(IE_OK, 1.1e4932L);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);

    fclose(input);
}