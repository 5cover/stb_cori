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

#define STB_CORI_VERSION "1.2.0"

#define __STDC_WANT_LIB_EXT2__ 1

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This is needed to use getdelim in a standard-compliant way
#if _POSIX_C_SOURCE >= 200809L || defined __STDC_ALLOC_LIB__
#define CORI_GETDELIM
#endif

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

CORI_DEFINITION void cori_handle_error_noop(InputError);
CORI_DEFINITION void cori_handle_error_stderr_msg(InputError);

/// \brief Helps pass commas in macro arguments.
#define _cori_COMMA ,

#if defined CORI_GETDELIM
#define _cori_FOREACH_CONVERSION(DO, DO_WITH_ARGS)                                       \
    /* Signed integer */                                                                 \
    DO(_cori_read_rawLine, intmax, intmax_t)                                             \
    DO(_cori_read_rawLine, longlong, long long)                                          \
    DO(_cori_read_rawLine, long, long)                                                   \
    DO(_cori_read_rawLine, int, int)                                                     \
    DO(_cori_read_rawLine, short, short)                                                 \
    DO_WITH_ARGS(_cori_read_rawLine, intmax_base, intmax_t, int base, base)              \
    DO_WITH_ARGS(_cori_read_rawLine, longlong_base, long long, int base, base)           \
    DO_WITH_ARGS(_cori_read_rawLine, long_base, long, int base, base)                    \
    DO_WITH_ARGS(_cori_read_rawLine, int_base, int, int base, base)                      \
    DO_WITH_ARGS(_cori_read_rawLine, short_base, short, int base, base)                  \
    /* Unsigned integer */                                                               \
    DO(_cori_read_rawLine, uintmax, uintmax_t)                                           \
    DO(_cori_read_rawLine, ulonglong, unsigned long long)                                \
    DO(_cori_read_rawLine, ulong, unsigned long)                                         \
    DO(_cori_read_rawLine, uint, unsigned int)                                           \
    DO(_cori_read_rawLine, ushort, unsigned short)                                       \
    DO_WITH_ARGS(_cori_read_rawLine, uintmax_base, uintmax_t, int base, base)            \
    DO_WITH_ARGS(_cori_read_rawLine, ulonglong_base, unsigned long long, int base, base) \
    DO_WITH_ARGS(_cori_read_rawLine, ulong_base, unsigned long, int base, base)          \
    DO_WITH_ARGS(_cori_read_rawLine, uint_base, unsigned int, int base, base)            \
    DO_WITH_ARGS(_cori_read_rawLine, ushort_base, unsigned short, int base, base)        \
    /* Floating point */                                                                 \
    DO(_cori_read_rawLine, longdouble, long double)                                      \
    DO(_cori_read_rawLine, double, double)                                               \
    DO(_cori_read_rawLine, float, float)                                                 \
    /* Text */                                                                           \
    DO(_cori_read_rawLine, line, char *)                                                 \
    DO_WITH_ARGS(_cori_read_rawDelim, until, char *, char delimiter, delimiter)          \
    DO(_cori_read_rawLine, character, char)                                              \
    /* Other */                                                                          \
    DO_WITH_ARGS(_cori_read_rawLine, bool, bool, char const *yesChars _cori_COMMA char const *noChars, yesChars _cori_COMMA noChars)
#else
#define _cori_FOREACH_CONVERSION(DO, DO_WITH_ARGS)                                       \
    /* Signed integer */                                                                 \
    DO(_cori_read_rawLine, intmax, intmax_t)                                             \
    DO(_cori_read_rawLine, longlong, long long)                                          \
    DO(_cori_read_rawLine, long, long)                                                   \
    DO(_cori_read_rawLine, int, int)                                                     \
    DO(_cori_read_rawLine, short, short)                                                 \
    DO_WITH_ARGS(_cori_read_rawLine, intmax_base, intmax_t, int base, base)              \
    DO_WITH_ARGS(_cori_read_rawLine, longlong_base, long long, int base, base)           \
    DO_WITH_ARGS(_cori_read_rawLine, long_base, long, int base, base)                    \
    DO_WITH_ARGS(_cori_read_rawLine, int_base, int, int base, base)                      \
    DO_WITH_ARGS(_cori_read_rawLine, short_base, short, int base, base)                  \
    /* Unsigned integer */                                                               \
    DO(_cori_read_rawLine, uintmax, uintmax_t)                                           \
    DO(_cori_read_rawLine, ulonglong, unsigned long long)                                \
    DO(_cori_read_rawLine, ulong, unsigned long)                                         \
    DO(_cori_read_rawLine, uint, unsigned int)                                           \
    DO(_cori_read_rawLine, ushort, unsigned short)                                       \
    DO_WITH_ARGS(_cori_read_rawLine, uintmax_base, uintmax_t, int base, base)            \
    DO_WITH_ARGS(_cori_read_rawLine, ulonglong_base, unsigned long long, int base, base) \
    DO_WITH_ARGS(_cori_read_rawLine, ulong_base, unsigned long, int base, base)          \
    DO_WITH_ARGS(_cori_read_rawLine, uint_base, unsigned int, int base, base)            \
    DO_WITH_ARGS(_cori_read_rawLine, ushort_base, unsigned short, int base, base)        \
    /* Floating point */                                                                 \
    DO(_cori_read_rawLine, longdouble, long double)                                      \
    DO(_cori_read_rawLine, double, double)                                               \
    DO(_cori_read_rawLine, float, float)                                                 \
    /* Text */                                                                           \
    DO(_cori_read_rawLine, line, char *)                                                 \
    /*DO_WITH_ARGS(_cori_read_rawDelim, until, char *, char delimiter, delimiter)*/      \
    DO(_cori_read_rawLine, character, char)                                              \
    /* Other */                                                                          \
    DO_WITH_ARGS(_cori_read_rawLine, bool, bool, char const *yesChars _cori_COMMA char const *noChars, yesChars _cori_COMMA noChars)
#endif

#define _cori_DECLARE_READ_FUNCS(readRawFunc, typename, type)                            \
    CORI_DEFINITION InputError tryRead_##typename(type *);                               \
    CORI_DEFINITION InputError tryRead_##typename##_from(type *, FILE *);                \
    CORI_DEFINITION type read_##typename(void);                                          \
    CORI_DEFINITION type read_##typename##_from(FILE *);                                 \
    CORI_DEFINITION type read_##typename##_or_from(InputErrorHandler, FILE *); \
    CORI_DEFINITION type read_##typename##_or(InputErrorHandler);

#define _cori_DECLARE_READ_FUNCS_WITH_ARGS(readRawFunc, typename, type, typedArgs, args)            \
    CORI_DEFINITION InputError tryRead_##typename(type *, typedArgs);                               \
    CORI_DEFINITION InputError tryRead_##typename##_from(type *, typedArgs, FILE *);                \
    CORI_DEFINITION type read_##typename(typedArgs);                                                \
    CORI_DEFINITION type read_##typename##_from(typedArgs, FILE *);                                 \
    CORI_DEFINITION type read_##typename##_or_from(typedArgs, InputErrorHandler, FILE *); \
    CORI_DEFINITION type read_##typename##_or(typedArgs, InputErrorHandler);

_cori_FOREACH_CONVERSION(_cori_DECLARE_READ_FUNCS, _cori_DECLARE_READ_FUNCS_WITH_ARGS)

    // Necessary as strtoul functions don't handle negative input
    CORI_DEFINITION InputError _cori_deny_negative(char const *);

CORI_DEFINITION char *read_linel_from(char *, size_t, FILE *);
CORI_DEFINITION char *read_linel_or_from(char *, size_t, InputErrorHandler, FILE *);
CORI_DEFINITION char *read_linel_or(char *, size_t, InputErrorHandler);
CORI_DEFINITION char *read_linel(char *, size_t);
CORI_DEFINITION InputError tryRead_linel_from(char *, size_t, FILE *);
CORI_DEFINITION InputError tryRead_linel(char *, size_t);

#endif // STB_CORI_H_

#ifdef CORI_IMPLEMENTATION

// Utilities

CORI_DEFINITION bool _cori_is_stringLength_exactly(char const *str, size_t length)
{
    size_t measuredLength = 0;
    char const *pCurrent = str;
    while (measuredLength < length && *pCurrent != '\0') {
        ++measuredLength;
        ++pCurrent;
    }
    return length == measuredLength && *pCurrent == '\0';
}

CORI_DEFINITION InputError _cori_get_numberConversionError(char const *str, char const *end)
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

CORI_DEFINITION InputError _cori_getCharacterConversionError(char const *buffer)
{
    // Accout for terminating newline
    if (_cori_is_stringLength_exactly(buffer, 1)) {
        return IE_EMPTY;
    } else if (!_cori_is_stringLength_exactly(buffer, 2)) {
        return IE_MULTIPLE_CHARS;
    }
    return IE_OK;
}

#if defined CORI_GETDELIM
CORI_DEFINITION InputError _cori_read_rawDelim(char **pBuffer, size_t *pBufferSize, FILE *stream, int delimiter, ...)
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

CORI_DEFINITION InputError _cori_read_rawLine(char **pBuffer, size_t *pBufferSize, FILE *stream, ...)
{
    return _cori_read_rawDelim(pBuffer, pBufferSize, stream, '\n');
}
#endif

CORI_DEFINITION void _cori_remove_lastChar(char *str)
{
    assert(str != NULL);
    str[strlen(str) - 1] = '\0';
}

CORI_DEFINITION InputError _cori_deny_negative(char const *str)
{
    return str[0] == '-' ? IE_NUMBER_OUT_OF_BOUNDS : IE_OK;
}

// Read functions definitions

CORI_DEFINITION InputError _cori_tryRead_linel_impl(char *buf, size_t bufsize, FILE *stream)
{
    if (fgets(buf, bufsize, stream) == NULL) {
        return IE_EOF;
    }
    return IE_OK;
}

char *read_linel_or_from(char *buf, size_t bufsize, InputErrorHandler inputErrorHandler, FILE *stream)
{
    InputError error;
    while ((error = _cori_tryRead_linel_impl(buf, bufsize, stream))) {
        inputErrorHandler(error);
    }
    return buf;
}
InputError tryRead_linel_from(char *buf, size_t bufsize, FILE *stream)
{
    return _cori_tryRead_linel_impl(buf, bufsize, stream);
}
char *read_linel_from(char *buf, size_t bufsize, FILE *stream)
{
    return read_linel_or_from(buf, bufsize, cori_handle_error_noop, stream);
}
char *read_linel(char *buf, size_t bufsize)
{
    return read_linel_or_from(buf, bufsize, cori_handle_error_noop, stdin);
}
char *read_linel_or(char *buf, size_t bufsize, InputErrorHandler inputErrorHandler)
{
    return read_linel_or_from(buf, bufsize, inputErrorHandler, stdin);
}
InputError tryRead_linel(char *buf, size_t bufsize)
{
    return tryRead_linel_from(buf, bufsize, stdin);
}

// INTERNAL

// Converters

CORI_DEFINITION InputError _cori_convert_bool(char const *str, bool *outResult, char const *yesChars, char const *noChars)
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

CORI_DEFINITION InputError _cori_convert_character(char const *str, char *outResult)
{
    InputError error = _cori_getCharacterConversionError(str);
    if (!error) {
        *outResult = str[0];
    }
    return error;
}

CORI_DEFINITION InputError _cori_convert_until(char const *str, char **outResult, char delimiter)
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

CORI_DEFINITION InputError _cori_convert_line(char const *str, char **outResult)
{
    *outResult = strdup(str);
    _cori_remove_lastChar(*outResult);
    return IE_OK;
}

// Standard library numeric conversion wrappers

CORI_DEFINITION InputError _cori_convert_intmax_base(char const *str, intmax_t *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_intmax(char const *str, intmax_t *outResult)
{
    return _cori_convert_intmax_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_longlong_base(char const *str, long long *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_longlong(char const *str, long long *outResult)
{
    return _cori_convert_longlong_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_long_base(char const *str, long *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_long(char const *str, long *outResult)
{
    return _cori_convert_long_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_uintmax_base(char const *str, uintmax_t *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_uintmax(char const *str, uintmax_t *outResult)
{
    return _cori_convert_uintmax_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_ulonglong_base(char const *str, unsigned long long *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_ulonglong(char const *str, unsigned long long *outResult)
{
    return _cori_convert_ulonglong_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_ulong_base(char const *str, unsigned long *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_ulong(char const *str, unsigned long *outResult)
{
    return _cori_convert_ulong_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_longdouble(char const *str, long double *outResult)
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

CORI_DEFINITION InputError _cori_convert_double(char const *str, double *outResult)
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

CORI_DEFINITION InputError _cori_convert_float(char const *str, float *outResult)
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

// Custom numeric converters

CORI_DEFINITION InputError _cori_convert_int_base(char const *str, int *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_int(char const *str, int *outResult)
{
    return _cori_convert_int_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_short_base(char const *str, short *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_short(char const *str, short *outResult)
{
    return _cori_convert_short_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_uint_base(char const *str, unsigned int *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_uint(char const *str, unsigned int *outResult)
{
    return _cori_convert_uint_base(str, outResult, 0);
}

CORI_DEFINITION InputError _cori_convert_ushort_base(char const *str, unsigned short *outResult, int base)
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

CORI_DEFINITION InputError _cori_convert_ushort(char const *str, unsigned short *outResult)
{
    return _cori_convert_ushort_base(str, outResult, 0);
}

// Error handlers

void cori_handle_error_noop(InputError error)
{
    (void)error;
}

void cori_handle_error_stderr_msg(InputError error)
{
    fprintf(stderr, "input error: ");
    switch (error) {
    case IE_EMPTY: fputs("empty input", stderr); break;
    case IE_EOF: fputs("EOF reached", stderr); break;
    case IE_INVALID_BOOL_CHAR: fputs("invalid boolean char", stderr); break;
    case IE_MULTIPLE_CHARS: fputs("multiple chars", stderr); break;
    case IE_NOT_A_NUMBER: fputs("not a number", stderr); break;
    case IE_NUMBER_OUT_OF_BOUNDS: fputs("number out of bounds", stderr); break;
    case IE_OUT_OF_MEMORY: fputs("out of memory", stderr); break;
    case IE_UNSUPPORTED_BASE: fputs("unsupported numeric base", stderr); break;
    default: fprintf(stderr, "%d", error); break;
    }
    putc('\n', stderr);
}

// Conversions definitions

#define _cori_DEFINE_READ_FUNCS(readRawFunc, typename, type)                                                                       \
    CORI_DEFINITION InputError _cori_tryRead_##typename##_impl(char **pBuffer, size_t *pBufferSize, type *outResult, FILE *stream) \
    {                                                                                                                              \
        InputError error = readRawFunc(pBuffer, pBufferSize, stream);                                                              \
        if (error) {                                                                                                               \
            return error;                                                                                                          \
        }                                                                                                                          \
        type result;                                                                                                               \
        error = _cori_convert_##typename(*pBuffer, &result);                                                                       \
        if (error) {                                                                                                               \
            return error;                                                                                                          \
        }                                                                                                                          \
        *outResult = result;                                                                                                       \
        return IE_OK;                                                                                                              \
    }                                                                                                                              \
    type read_##typename##_or_from(InputErrorHandler inputErrorHandler, FILE *stream)                                    \
    {                                                                                                                              \
        char *buffer = NULL;                                                                                                       \
        size_t bufferSize = 0;                                                                                                     \
        type result;                                                                                                               \
        InputError error;                                                                                                          \
        while ((error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, &result, stream))) {                                 \
            inputErrorHandler(error);                                                                                              \
        }                                                                                                                          \
        free(buffer);                                                                                                              \
        return result;                                                                                                             \
    }                                                                                                                              \
    InputError tryRead_##typename##_from(type *outResult, FILE *stream)                                                            \
    {                                                                                                                              \
        char *buffer = NULL;                                                                                                       \
        size_t bufferSize = 0;                                                                                                     \
        InputError error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, outResult, stream);                               \
        free(buffer);                                                                                                              \
        return error;                                                                                                              \
    }                                                                                                                              \
    type read_##typename##_from(FILE *stream)                                                                                      \
    {                                                                                                                              \
        return read_##typename##_or_from(cori_handle_error_noop, stream);                                               \
    }                                                                                                                              \
    type read_##typename(void)                                                                                                     \
    {                                                                                                                              \
        return read_##typename##_or_from(cori_handle_error_noop, stdin);                                                \
    }                                                                                                                              \
    type read_##typename##_or(InputErrorHandler inputErrorHandler)                                                       \
    {                                                                                                                              \
        return read_##typename##_or_from(inputErrorHandler, stdin);                                                      \
    }                                                                                                                              \
    InputError tryRead_##typename(type * outResult)                                                                                \
    {                                                                                                                              \
        return tryRead_##typename##_from(outResult, stdin);                                                                        \
    }

#define _cori_DEFINE_READ_FUNCS_WITH_ARGS(readRawFunc, typename, type, typedArgs, args)                                                       \
    CORI_DEFINITION InputError _cori_tryRead_##typename##_impl(char **pBuffer, size_t *pBufferSize, type *outResult, typedArgs, FILE *stream) \
    {                                                                                                                                         \
        InputError error = readRawFunc(pBuffer, pBufferSize, stream, args);                                                                   \
        if (error) {                                                                                                                          \
            return error;                                                                                                                     \
        }                                                                                                                                     \
        type result;                                                                                                                          \
        error = _cori_convert_##typename(*pBuffer, &result, args);                                                                            \
        if (error) {                                                                                                                          \
            return error;                                                                                                                     \
        }                                                                                                                                     \
        *outResult = result;                                                                                                                  \
        return IE_OK;                                                                                                                         \
    }                                                                                                                                         \
    type read_##typename##_or_from(typedArgs, InputErrorHandler inputErrorHandler, FILE *stream)                                    \
    {                                                                                                                                         \
        char *buffer = NULL;                                                                                                                  \
        size_t bufferSize = 0;                                                                                                                \
        type result;                                                                                                                          \
        InputError error;                                                                                                                     \
        while ((error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, &result, args, stream))) {                                      \
            inputErrorHandler(error);                                                                                                         \
        }                                                                                                                                     \
        free(buffer);                                                                                                                         \
        return result;                                                                                                                        \
    }                                                                                                                                         \
    InputError tryRead_##typename##_from(type *outResult, typedArgs, FILE *stream)                                                            \
    {                                                                                                                                         \
        char *buffer = NULL;                                                                                                                  \
        size_t bufferSize = 0;                                                                                                                \
        InputError error = _cori_tryRead_##typename##_impl(&buffer, &bufferSize, outResult, args, stream);                                    \
        free(buffer);                                                                                                                         \
        return error;                                                                                                                         \
    }                                                                                                                                         \
    type read_##typename##_from(typedArgs, FILE *stream)                                                                                      \
    {                                                                                                                                         \
        return read_##typename##_or_from(args, cori_handle_error_noop, stream);                                                    \
    }                                                                                                                                         \
    type read_##typename(typedArgs)                                                                                                           \
    {                                                                                                                                         \
        return read_##typename##_or_from(args, cori_handle_error_noop, stdin);                                                     \
    }                                                                                                                                         \
    type read_##typename##_or(typedArgs, InputErrorHandler inputErrorHandler)                                                       \
    {                                                                                                                                         \
        return read_##typename##_or_from(args, inputErrorHandler, stdin);                                                           \
    }                                                                                                                                         \
    InputError tryRead_##typename(type * outResult, typedArgs)                                                                                \
    {                                                                                                                                         \
        return tryRead_##typename##_from(outResult, args, stdin);                                                                             \
    }

_cori_FOREACH_CONVERSION(_cori_DEFINE_READ_FUNCS, _cori_DEFINE_READ_FUNCS_WITH_ARGS)

#endif // CORI_IMPLEMENTATION
