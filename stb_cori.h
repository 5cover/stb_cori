/** \file
 * \author 5cover (Scover)
 * \brief Helper console input functions with error handling.
 * \copyright Public Domain - The Unlicense
 * \date 3/10/2023
 * \details
 * See README.md for details and usage.
 */

#ifndef STB_CORI_H_
#define STB_CORI_H_

// This is needed to use getdelim in a standard-compliant way
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#elif !defined _POSIX_C_SOURCE // Don't overwrite value given by implementing code
#define _POSIX_C_SOURCE 200809L
#endif

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CORI_DEFINITION
/// \brief Defines modifier keywords for the definitions. Example value: static inline
#define CORI_DEFINITION
#endif

typedef enum {
    /// \brief No error.
    IE_OK = 0,
    /// \brief Issued string empty.
    IE_EMPTY,
    /// \brief EOF encountered while reading.
    IE_EOF,
    /// \brief Unrecognized boolean char.
    IE_INVALID_BOOL_CHAR,
    /// \brief Multiple chars were issued.
    IE_MULTIPLE_CHARS,
    /// \brief Issued value is not a number.
    IE_NOT_A_NUMBER,
    /// \brief Issued number is out of bounds.
    IE_NUMBER_OUT_OF_BOUNDS,
    /// \brief Out of memory condition.
    IE_OUT_OF_MEMORY,
    /// \brief Unsupported numeric base.
    IE_UNSUPPORTED_BASE
} InputError;

/// \brief Input error handler procedure.
typedef void (*InputErrorHandler)(InputError);

/// \brief Helps pass commas in macro arguments.
#define _cori_COMMA ,

#define _cori_FOR_EACH_CONVERSION(DO, DO_WITH_ARGS)                                       \
    /* Signed integer */                                                                  \
    DO(_cori_read_rawLine, intmax, intmax_t)                                              \
    DO(_cori_read_rawLine, longlong, long long)                                           \
    DO(_cori_read_rawLine, long, long)                                                    \
    DO(_cori_read_rawLine, int, int)                                                      \
    DO(_cori_read_rawLine, short, short)                                                  \
    DO_WITH_ARGS(_cori_read_rawLine, intmax_base, intmax_t, int base, base)               \
    DO_WITH_ARGS(_cori_read_rawLine, longlong_base, long long, int base, base)            \
    DO_WITH_ARGS(_cori_read_rawLine, long_base, long, int base, base)                     \
    DO_WITH_ARGS(_cori_read_rawLine, int_base, int, int base, base)                       \
    DO_WITH_ARGS(_cori_read_rawLine, short_base, short, int base, base)                   \
    /* Unsigned integer */                                                                \
    DO(_cori_read_rawLine, uintmax, uintmax_t)                                            \
    DO(_cori_read_rawLine, ulonglong, unsigned long long)                                 \
    DO(_cori_read_rawLine, ulong, unsigned long)                                          \
    DO(_cori_read_rawLine, uint, unsigned int)                                            \
    DO(_cori_read_rawLine, ushort, unsigned short)                                        \
    DO_WITH_ARGS(_cori_read_rawLine, uintmax_base, uintmax_t, int base, base)             \
    DO_WITH_ARGS(_cori_read_rawLine, ulonglong_base, unsigned long long, int base, base)  \
    DO_WITH_ARGS(_cori_read_rawLine, ulong_base, unsigned long, int base, base)           \
    DO_WITH_ARGS(_cori_read_rawLine, uint_base, unsigned int, int base, base)             \
    DO_WITH_ARGS(_cori_read_rawLine, ushort_base, unsigned short, int base, base)         \
    /* Floating point */                                                                  \
    DO(_cori_read_rawLine, longdouble, long double)                                       \
    DO(_cori_read_rawLine, double, double)                                                \
    DO(_cori_read_rawLine, float, float)                                                  \
    /* Text */                                                                            \
    DO(_cori_read_rawLine, line, char *)                                                  \
    DO_WITH_ARGS(_cori_read_rawDelim, delimitedString, char *, char delimiter, delimiter) \
    DO(_cori_read_rawLine, character, char)                                               \
    /* Other */                                                                           \
    DO_WITH_ARGS(_cori_read_rawLine, bool, bool, char const *yesChars _cori_COMMA char const *noChars, yesChars _cori_COMMA noChars)

#define _cori_DECLARE_READ_FUNCS(readRawFunc, typename, type)                                                    \
    CORI_DEFINITION InputError tryRead_##typename(type * outResult);                                             \
    CORI_DEFINITION InputError tryRead_##typename##_from(type *outResult, FILE *stream);                         \
    CORI_DEFINITION type read_##typename(void);                                                                  \
    CORI_DEFINITION type read_##typename##_from(FILE *stream);                                                   \
    CORI_DEFINITION type read_##typename##_handleErrors_from(InputErrorHandler inputErrorHandler, FILE *stream); \
    CORI_DEFINITION type read_##typename##_handleErrors(InputErrorHandler inputErrorHandler);                    \
    CORI_DEFINITION InputError _cori_convert_##typename(char const *str, type *outResult);                       \
    CORI_DEFINITION InputError _cori_tryRead_##typename##_impl(char **pBuffer, size_t *pBufferSize, type *outResult, FILE *stream);

#define _cori_DECLARE_READ_FUNCS_WITH_ARGS(readRawFunc, typename, type, typedArgs, args)                                    \
    CORI_DEFINITION InputError tryRead_##typename(type * outResult, typedArgs);                                             \
    CORI_DEFINITION InputError tryRead_##typename##_from(type *outResult, typedArgs, FILE *stream);                         \
    CORI_DEFINITION type read_##typename(typedArgs);                                                                        \
    CORI_DEFINITION type read_##typename##_from(typedArgs, FILE *stream);                                                   \
    CORI_DEFINITION type read_##typename##_handleErrors_from(typedArgs, InputErrorHandler inputErrorHandler, FILE *stream); \
    CORI_DEFINITION type read_##typename##_handleErrors(typedArgs, InputErrorHandler inputErrorHandler);                    \
    CORI_DEFINITION InputError _cori_convert_##typename(char const *str, type *outResult, typedArgs);                       \
    CORI_DEFINITION InputError _cori_tryRead_##typename##_impl(char **pBuffer, size_t *pBufferSize, type *outResult, typedArgs, FILE *stream);

_cori_FOR_EACH_CONVERSION(_cori_DECLARE_READ_FUNCS, _cori_DECLARE_READ_FUNCS_WITH_ARGS)

    CORI_DEFINITION void handle_inputError(InputError error);

// INTERNAL

CORI_DEFINITION bool _cori_is_stringLength_exactly(char const *, size_t);

CORI_DEFINITION InputError _cori_getCharacterConversionError(char const *);

CORI_DEFINITION InputError _cori_get_numberConversionError(char const *, char const *);

CORI_DEFINITION InputError _cori_read_rawDelim(char **, size_t *, FILE *, int, ...);

CORI_DEFINITION InputError _cori_read_rawLine(char **, size_t *, FILE *, ...);

CORI_DEFINITION void _cori_remove_lastChar(char *);

// Necessary as strtoul functions don't handle negative input
CORI_DEFINITION InputError _cori_deny_negative(char const *);

#endif // STB_CORI_H_

#ifdef CORI_IMPLEMENTATION

// Read functions definitions

#define _cori_DEFINE_READ_FUNCS(readRawFunc, typename, type)                                                       \
    type read_##typename##_handleErrors_from(InputErrorHandler inputErrorHandler, FILE *stream)                    \
    {                                                                                                              \
        char *buffer = NULL;                                                                                       \
        size_t bufferSize = 0;                                                                                     \
        type result;                                                                                               \
        InputError error;                                                                                          \
        while ((error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, &result, stream))) {                 \
            inputErrorHandler(error);                                                                              \
        }                                                                                                          \
        free(buffer);                                                                                              \
        return result;                                                                                             \
    }                                                                                                              \
    InputError tryRead_##typename##_from(type *outResult, FILE *stream)                                            \
    {                                                                                                              \
        char *buffer = NULL;                                                                                       \
        size_t bufferSize = 0;                                                                                     \
        InputError error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, outResult, stream);               \
        free(buffer);                                                                                              \
        return error;                                                                                              \
    }                                                                                                              \
    type read_##typename##_from(FILE *stream)                                                                      \
    {                                                                                                              \
        return read_##typename##_handleErrors_from(handle_inputError, stream);                                     \
    }                                                                                                              \
    type read_##typename(void)                                                                                     \
    {                                                                                                              \
        return read_##typename##_handleErrors_from(handle_inputError, stdin);                                      \
    }                                                                                                              \
    type read_##typename##_handleErrors(InputErrorHandler inputErrorHandler)                                       \
    {                                                                                                              \
        return read_##typename##_handleErrors_from(inputErrorHandler, stdin);                                      \
    }                                                                                                              \
    InputError tryRead_##typename(type * outResult)                                                                \
    {                                                                                                              \
        return tryRead_##typename##_from(outResult, stdin);                                                        \
    }                                                                                                              \
    InputError _cori_tryRead_##typename##_impl(char **pBuffer, size_t *pBufferSize, type *outResult, FILE *stream) \
    {                                                                                                              \
        InputError error = readRawFunc(pBuffer, pBufferSize, stream);                                              \
        if (error) {                                                                                               \
            return error;                                                                                          \
        }                                                                                                          \
        type result;                                                                                               \
        error = _cori_convert_##typename(*pBuffer, &result);                                                       \
        if (error) {                                                                                               \
            return error;                                                                                          \
        }                                                                                                          \
        *outResult = result;                                                                                       \
        return IE_OK;                                                                                              \
    }

#define _cori_DEFINE_READ_FUNCS_WITH_ARGS(readRawFunc, typename, type, typedArgs, args)                                       \
    type read_##typename##_handleErrors_from(typedArgs, InputErrorHandler inputErrorHandler, FILE *stream)                    \
    {                                                                                                                         \
        char *buffer = NULL;                                                                                                  \
        size_t bufferSize = 0;                                                                                                \
        type result;                                                                                                          \
        InputError error;                                                                                                     \
        while ((error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, &result, args, stream))) {                      \
            inputErrorHandler(error);                                                                                         \
        }                                                                                                                     \
        free(buffer);                                                                                                         \
        return result;                                                                                                        \
    }                                                                                                                         \
    InputError tryRead_##typename##_from(type *outResult, typedArgs, FILE *stream)                                            \
    {                                                                                                                         \
        char *buffer = NULL;                                                                                                  \
        size_t bufferSize = 0;                                                                                                \
        InputError error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, outResult, args, stream);                    \
        free(buffer);                                                                                                         \
        return error;                                                                                                         \
    }                                                                                                                         \
    type read_##typename##_from(typedArgs, FILE *stream)                                                                      \
    {                                                                                                                         \
        return read_##typename##_handleErrors_from(args, handle_inputError, stream);                                          \
    }                                                                                                                         \
    type read_##typename(typedArgs)                                                                                           \
    {                                                                                                                         \
        return read_##typename##_handleErrors_from(args, handle_inputError, stdin);                                           \
    }                                                                                                                         \
    type read_##typename##_handleErrors(typedArgs, InputErrorHandler inputErrorHandler)                                       \
    {                                                                                                                         \
        return read_##typename##_handleErrors_from(args, inputErrorHandler, stdin);                                           \
    }                                                                                                                         \
    InputError tryRead_##typename(type * outResult, typedArgs)                                                                \
    {                                                                                                                         \
        return tryRead_##typename##_from(outResult, args, stdin);                                                             \
    }                                                                                                                         \
    InputError _cori_tryRead_##typename##_impl(char **pBuffer, size_t *pBufferSize, type *outResult, typedArgs, FILE *stream) \
    {                                                                                                                         \
        InputError error = readRawFunc(pBuffer, pBufferSize, stream, args);                                                   \
        if (error) {                                                                                                          \
            return error;                                                                                                     \
        }                                                                                                                     \
        type result;                                                                                                          \
        error = _cori_convert_##typename(*pBuffer, &result, args);                                                            \
        if (error) {                                                                                                          \
            return error;                                                                                                     \
        }                                                                                                                     \
        *outResult = result;                                                                                                  \
        return IE_OK;                                                                                                         \
    }

_cori_FOR_EACH_CONVERSION(_cori_DEFINE_READ_FUNCS, _cori_DEFINE_READ_FUNCS_WITH_ARGS)

// Skip if already defined by implementing code
#ifndef CORI_DEFAULT_INPUT_ERROR_HANDLER
    void handle_inputError(InputError error)
{
    (void)error;
}
#endif

// INTERNAL

// Converters

InputError _cori_convert_bool(char const *str, bool *outResult, char const *yesChars, char const *noChars)
{
    InputError error = _cori_getCharacterConversionError(str);
    if (error) {
        return error;
    }
    if (strchr(yesChars, str[0])) {
        *outResult = true;
    } else if (strchr(noChars, str[0])) {
        *outResult = false;
    } else {
        error = IE_INVALID_BOOL_CHAR;
    }
    return error;
}

InputError _cori_convert_character(char const *str, char *outResult)
{
    InputError error = _cori_getCharacterConversionError(str);
    if (!error) {
        *outResult = str[0];
    }
    return error;
}

InputError _cori_convert_delimitedString(char const *str, char **outResult, char delimiter)
{
    (void)delimiter;
    // Duplicate the string as the orginal one will be freed.
    *outResult = strdup(str);
    // remove the delimiter character
    _cori_remove_lastChar(*outResult);
    // Warning: conversion functions that perform memory allocation cause a memory leak if they return something else than IE_OK after performing the allocation.
    // This is because tryRead_typename_impl will return early, cause outResult to never be set, therefore not giving the chance to the user to call free.
    return IE_OK;
}

InputError _cori_convert_line(char const *str, char **outResult)
{
    *outResult = strdup(str);
    _cori_remove_lastChar(*outResult);
    return IE_OK;
}

// Custom numeric converters

InputError _cori_convert_int(char const *str, int *outResult)
{
    return _cori_convert_int_base(str, outResult, 0);
}

InputError _cori_convert_int_base(char const *str, int *outResult, int base)
{
    long result;
    InputError error = _cori_convert_long_base(str, &result, base);
    if (error) {
        return error;
    }
    if (result < INT_MIN || result > INT_MAX) {
        return IE_NUMBER_OUT_OF_BOUNDS;
    }
    *outResult = (int)result;
    return IE_OK;
}

InputError _cori_convert_short(char const *str, short *outResult)
{
    return _cori_convert_short_base(str, outResult, 0);
}

InputError _cori_convert_short_base(char const *str, short *outResult, int base)
{
    long result;
    InputError error = _cori_convert_long_base(str, &result, base);
    if (error) {
        return error;
    }
    if (result < SHRT_MIN || result > SHRT_MAX) {
        return IE_NUMBER_OUT_OF_BOUNDS;
    }
    *outResult = (short)result;
    return IE_OK;
}

InputError _cori_convert_uint(char const *str, unsigned int *outResult)
{
    return _cori_convert_uint_base(str, outResult, 0);
}

InputError _cori_convert_uint_base(char const *str, unsigned int *outResult, int base)
{
    unsigned long result;
    InputError error = _cori_convert_ulong_base(str, &result, base);
    if (error) {
        return error;
    }
    if (result > UINT_MAX) {
        return IE_NUMBER_OUT_OF_BOUNDS;
    }
    *outResult = (unsigned int)result;
    return IE_OK;
}

InputError _cori_convert_ushort(char const *str, unsigned short *outResult)
{
    return _cori_convert_ushort_base(str, outResult, 0);
}

InputError _cori_convert_ushort_base(char const *str, unsigned short *outResult, int base)
{
    unsigned long result;
    InputError error = _cori_convert_ulong_base(str, &result, base);
    if (error) {
        return error;
    }
    if (result > USHRT_MAX) {
        return IE_NUMBER_OUT_OF_BOUNDS;
    }
    *outResult = (unsigned short)result;
    return IE_OK;
}

// Standard library numeric conversion wrappers

InputError _cori_convert_intmax(char const *str, intmax_t *outResult)
{
    return _cori_convert_intmax_base(str, outResult, 0);
}

InputError _cori_convert_intmax_base(char const *str, intmax_t *outResult, int base)
{
    errno = 0;
    char *end = NULL;
    intmax_t result = strtoimax(str, &end, base);
    InputError error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_longlong(char const *str, long long *outResult)
{
    return _cori_convert_longlong_base(str, outResult, 0);
}

InputError _cori_convert_longlong_base(char const *str, long long *outResult, int base)
{
    errno = 0;
    char *end = NULL;
    long long result = strtoll(str, &end, base);
    InputError error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_long(char const *str, long *outResult)
{
    return _cori_convert_long_base(str, outResult, 0);
}

InputError _cori_convert_long_base(char const *str, long *outResult, int base)
{
    errno = 0;
    char *end = NULL;
    long result = strtol(str, &end, base);
    InputError error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_uintmax(char const *str, uintmax_t *outResult)
{
    return _cori_convert_uintmax_base(str, outResult, 0);
}

InputError _cori_convert_uintmax_base(char const *str, uintmax_t *outResult, int base)
{
    InputError error = _cori_deny_negative(str);
    if (error) {
        return error;
    }
    errno = 0;
    char *end = NULL;
    uintmax_t result = strtoumax(str, &end, base);
    error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_ulonglong(char const *str, unsigned long long *outResult)
{
    return _cori_convert_ulonglong_base(str, outResult, 0);
}

InputError _cori_convert_ulonglong_base(char const *str, unsigned long long *outResult, int base)
{
    InputError error = _cori_deny_negative(str);
    if (error) {
        return error;
    }
    errno = 0;
    char *end = NULL;
    unsigned long long result = strtoull(str, &end, base);
    error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_ulong(char const *str, unsigned long *outResult)
{
    return _cori_convert_ulong_base(str, outResult, 0);
}

InputError _cori_convert_ulong_base(char const *str, unsigned long *outResult, int base)
{
    InputError error = _cori_deny_negative(str);
    if (error) {
        return error;
    }
    errno = 0;
    char *end = NULL;
    unsigned long result = strtoul(str, &end, base);
    error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_longdouble(char const *str, long double *outResult)
{
    errno = 0;
    char *end = NULL;
    long double result = strtold(str, &end);
    InputError error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_double(char const *str, double *outResult)
{
    errno = 0;
    char *end = NULL;
    double result = strtod(str, &end);
    InputError error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

InputError _cori_convert_float(char const *str, float *outResult)
{
    errno = 0;
    char *end = NULL;
    float result = strtof(str, &end);
    InputError error = _cori_get_numberConversionError(str, end);
    if (!error) {
        *outResult = result;
    }
    return error;
}

// Utility functions

InputError _cori_get_numberConversionError(char const *str, char const *end)
{
    if (errno == ERANGE) {
        return IE_NUMBER_OUT_OF_BOUNDS;
    } else if (errno == EINVAL) {
        return IE_UNSUPPORTED_BASE;
    } else if (*end != '\0' && *end != '\n') {
        return IE_NOT_A_NUMBER;
    } else if (str == end) {
        return IE_EMPTY;
    }
    return IE_OK;
}

InputError _cori_getCharacterConversionError(char const *buffer)
{
    // Accout for terminating newline
    if (_cori_is_stringLength_exactly(buffer, 1)) {
        return IE_EMPTY;
    } else if (!_cori_is_stringLength_exactly(buffer, 2)) {
        return IE_MULTIPLE_CHARS;
    }
    return IE_OK;
}

InputError _cori_read_rawLine(char **pBuffer, size_t *pBufferSize, FILE *stream, ...)
{
    return _cori_read_rawDelim(pBuffer, pBufferSize, stream, '\n');
}

InputError _cori_read_rawDelim(char **pBuffer, size_t *pBufferSize, FILE *stream, int delimiter, ...)
{
    errno = 0;
    if (getdelim(pBuffer, pBufferSize, delimiter, stream) == -1) {
        if (errno == ENOMEM) {
            return IE_OUT_OF_MEMORY;
        }
        // Ctrl+D hit: push newline to avoid infinite loop.
        ungetc('\n', stream);
        // Read it back and trigger input prompt
        getc(stream);
        return IE_EOF;
    }
    return IE_OK;
}

void _cori_remove_lastChar(char *str)
{
    assert(str != NULL);
    str[strlen(str) - 1] = '\0';
}

bool _cori_is_stringLength_exactly(char const *str, size_t length)
{
    size_t measuredLength = 0;
    char const *pCurrent = str;
    while (measuredLength < length && *pCurrent != '\0') {
        ++measuredLength;
        ++pCurrent;
    }
    return length == measuredLength && *pCurrent == '\0';
}

InputError _cori_deny_negative(char const *str)
{
    return str[0] == '-' ? IE_NUMBER_OUT_OF_BOUNDS : IE_OK;
}

#endif // CORI_IMPLEMENTATION