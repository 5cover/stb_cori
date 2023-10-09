/** \file
 * \brief ushort test
 */

#include <limits.h>

#include "tests.h"

void test_ushort(FILE *output)
{
    FILE *input = open_inputFile("test_ushort");
#define UNSET 0
#define test(expectedError, expectedResult) test_tryRead(input, output, ushort, unsigned short, "%u", equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, 0);
    test(IE_OK, 43);
    test(IE_OK, 0x2B);
    test(IE_OK, USHRT_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}