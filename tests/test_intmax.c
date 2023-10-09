/** \file
 * \brief intmax test
 */

#include <inttypes.h>

#include "tests.h"

void test_intmax(FILE *output)
{
    FILE *input = open_inputFile("test_intmax");
#define UNSET INTMAX_C(0)
#define test(expectedError, expectedResult) test_tryRead(input, output, intmax, intmax_t, "%" PRIdMAX, equalsComparer, UNSET, expectedError, expectedResult)
    test(IE_OK, INTMAX_C(0));
    test(IE_OK, INTMAX_C(43));
    test(IE_OK, INTMAX_C(0x2B));
    test(IE_OK, INTMAX_MAX);
    test(IE_NUMBER_OUT_OF_BOUNDS, UNSET);
    test(IE_OK, INTMAX_C(-1));
    test(IE_EMPTY, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_NOT_A_NUMBER, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}