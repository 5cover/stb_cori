#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

TestingResults g_testingResults = (TestingResults){
    .failureCount = 0,
    .successCount = 0,
};

FILE *open_file(char const *filename, char const *mode)
{
    FILE *file = fopen(filename, mode);
    if (!file) {
        perror(filename);
        abort();
    }
    return file;
}