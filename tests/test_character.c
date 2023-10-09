/** \file
 * \brief character test
 */

#include "tests.h"

void test_character(FILE *output)
{
    FILE *input = open_inputFile("test_character");
#define UNSET 0
#define test(expectedError, expectedResult) test_tryRead(input, output, character, char, "'%c'", equalsComparer, UNSET, expectedError, expectedResult)

    test(IE_OK, 'c');
    test(IE_OK, 'd');
    test(IE_OK, ' ');
    test(IE_EMPTY, UNSET);
    test(IE_MULTIPLE_CHARS, UNSET);

    fclose(input);
}