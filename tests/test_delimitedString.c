/** \file
 * \brief delimitedString test
 */

#include "tests.h"

void test_delimitedString(FILE *output)
{
    FILE *input = open_inputFile("test_delimitedString");
#define UNSET ""
#define test(expectedError, expectedResult) test_tryRead(input, output, delimitedString, char *, "\"%s\"", stringComparer, UNSET, expectedError, expectedResult, '.')

    test(IE_OK, "hello world");
    test(IE_OK, " hello\n      world");
    test(IE_OK, "");
    test(IE_OK, "");
    test(IE_OK, "\n");
    test(IE_EOF, UNSET);

    fclose(input);
}