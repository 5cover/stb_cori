/** \file
 * \brief bool test
 */

#include "tests.h"

void test_bool(FILE *output)
{
    FILE *input = open_inputFile("test_bool");
#define UNSET false
#define test(expectedError, expectedResult) test_tryRead(input, output, bool, bool, "%d", equalsComparer, UNSET, expectedError, expectedResult, "yY", "nN")

    test(IE_OK, true);
    test(IE_OK, true);
    test(IE_OK, false);
    test(IE_OK, false);
    test(IE_INVALID_BOOL_CHAR, UNSET);
    test(IE_INVALID_BOOL_CHAR, UNSET);
    test(IE_EMPTY, UNSET);
    test(IE_MULTIPLE_CHARS, UNSET);
    test(IE_MULTIPLE_CHARS, UNSET);
    test(IE_MULTIPLE_CHARS, UNSET);
    test(IE_MULTIPLE_CHARS, UNSET);
    test(IE_EOF, UNSET);

    fclose(input);
}