/** \file
 * \brief longlong test
 */

#include <limits.h>

#include "tests.h"

void test_longlong(FILE *output)
{
    FILE *input = open_inputFile("test_longlong");
#define UNSET 0ll
#define test(expectedError, expectedResult) test_tryRead(input, output, longlong, long long, "%lld", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0ll);
    test(IE_OK, 43ll);
    test(IE_OK, 0x2Bll);
    test(IE_OK, LLONG_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_OK, -1ll);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}