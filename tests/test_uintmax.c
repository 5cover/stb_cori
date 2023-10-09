/** \file
 * \brief uintmax test
 */

#include <inttypes.h>

#include "tests.h"

void test_uintmax(FILE *output)
{
    FILE *input = open_inputFile("test_uintmax");
#define UNSET UINTMAX_C(0)
#define test(expectedError, expectedResult) test_tryRead(input, output, uintmax, uintmax_t, "%" PRIuMAX, equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, UINTMAX_C(0));
    test(IE_OK, UINTMAX_C(43));
    test(IE_OK, UINTMAX_C(0x2B));
    test(IE_OK, UINTMAX_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}