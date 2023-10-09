/** \file
 * \brief Specialized testing interface.
 */

#ifndef TESTS_H_
#define TESTS_H_

#include "../stb_cori.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define open_inputFile(testName) open_file(testName "_stdin.txt", "r")

typedef struct {
    size_t successCount;
    size_t failureCount;
} TestingResults;

extern TestingResults g_testingResults;

FILE *open_file(char const *filename, char const *mode);

#define test_tryRead(inStream, outStream, typename, resultType, resultFormat, resultComparer, unsetResult,             \
                     expectedError, expectedResult, ...)                                                               \
    do {                                                                                                               \
        resultType result = (unsetResult);                                                                             \
        InputError error = tryRead_##typename##_from(&result __VA_OPT__(, ) __VA_ARGS__, (inStream));                  \
        bool success = error == (expectedError) && resultComparer(result, (expectedResult));                           \
        fprintf(outStream, "%s: %d = %d and " resultFormat " = " resultFormat "\t%d:%s\n",                             \
                success ? (++g_testingResults.successCount, "SUCCESS") : (++g_testingResults.failureCount, "FAILURE"), \
                error, (expectedError), result, (expectedResult), __LINE__, __FILE__);                                 \
    } while (0)

#define equalsComparer(a, b) ((a) == (b))
#define floatComparer(a, b) ((isinf(a) && isinf(b)) || (isnan(a) && isnan(b)) || a == b)
#define stringComparer(a, b) (strcmp(a, b) == 0)

void test_character(FILE *);
void test_bool(FILE *);
void test_float(FILE *);
void test_double(FILE *);
void test_longdouble(FILE *);
void test_delimitedString(FILE *);
void test_intmax(FILE *);
void test_longlong(FILE *);
void test_long(FILE *);
void test_int(FILE *);
void test_short(FILE *);
void test_uintmax(FILE *);
void test_ulonglong(FILE *);
void test_ulong(FILE *);
void test_uint(FILE *);
void test_ushort(FILE *);

#endif // TESTS_H_
