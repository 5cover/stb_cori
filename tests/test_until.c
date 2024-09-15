/** \file
 * \brief until test
 */

#include "tests.h"

void test_until(FILE *output)
{
    FILE *input = open_inputFile("test_until");
#define UNSET ""
#define test(expectedError, expectedResult) test_tryRead(input, output, until, char *, "\"%s\"", stringComparer, UNSET, expectedError, expectedResult, '.')

    test(IE_OK, "hello world");
    test(IE_OK, " hello\n      world");
    test(IE_OK, "");
    test(IE_OK, "");
    test(IE_OK, "\n");
    test(IE_EOF, UNSET);

    fclose(input);
}