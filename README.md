# Cori

STB-style helper library for console input with error handling.

## Usage

Cori is basically a bunch of functions.

To choose the appropriate function, you can follow these steps:

### 1. What type of data do I need from the user?

Cori currently supports the following conversions:

> [!NOTE]
> For integer conversions, type names ending in *_base* indicate a ``base`` integer argument. The supported values are same as for the standard library's string to integer function family (``strtol``, ``strtoul``...).\
> Other type names assume 0 as the base, meaning the base is detected automatically.

#### Signed integers

Type name | Expected input | Actual type
-|-|-
intmax | An integer in range [``INTMAX_MIN`` ; ``INTMAX_MAX``] | ``intmax_t``
longlong | An integer in range [``LLONG_MIN`` ; ``LLONG_MAX``] | ``long long``
long | An integer in range [``LONG_MIN`` ; ``LONG_MAX``] | ``long``
int | An integer in range [``INT_MIN`` ; ``INT_MAX``] | ``int``
short | An integer in range [``SHRT_MIN`` ; ``SHRT_MAX``] | ``short``
intmax_base | An integer in range [``INTMAX_MIN`` ; ``INTMAX_MAX``] in the specified base | ``intmax_t``
longlong_base | An integer in range [``LLONG_MIN`` ; ``LLONG_MAX``] in the specified base | ``long long``
long_base | An integer in range [``LONG_MIN`` ; ``LONG_MAX``] in the specified base | ``long``
int_base | An integer in range [``INT_MIN`` ; ``INT_MAX``] in the specified base | ``int``
short_base | An integer in range [``SHRT_MIN`` ; ``SHRT_MAX``] in the specified base | ``short``

### Unsigned integers

Type name | Expected input | Actual type
-|-|-
uintmax | An integer in range [0 ; ``UINTMAX_MAX``] | ``uintmax_t``
ulonglong | An integer in range [0 ; ``ULLONG_MAX``] | ``unsigned long long``
ulong | An integer in range [0 ; ``ULONG_MAX``] | ``unsigned long``
uint | An integer in range [0 ; ``UINT_MAX``] | ``unsigned int``
ushort | An integer in range [0 ; ``USHRT_MAX``] | ``unsigned short``
uintmax_base | An integer in range [0 ; ``UINTMAX_MAX``] | ``uintmax_t``
ulonglong_base | An integer in range [0 ; ``ULLONG_MAX``] | ``unsigned long long``
ulong_base | An integer in range [0 ; ``ULONG_MAX``] | ``unsigned long``
uint_base | An integer in range [0 ; ``UINT_MAX``] | ``unsigned int``
ushort_base | An integer in range [0 ; ``USHRT_MAX``] | ``unsigned short``

### Floating point

Type name | Expected input | Actual type
-|-|-
longdouble | A real number | ``long double``
double | A real number | ``double``
float | A real number | ``float``

#### Text

Type name | Expected input | Actual type
-|-|-
character | Any character except a newline | ``char``
delimitedString | A string ended by the specified character | ``char *`` (must be freed)
line | A string ended by a newline | ``char *`` (must be freed)

#### Other

Type name | Expected input | Actual type
-|-|-
boolean | A character, present in one of the specified sets of characters representing either ``true`` or ``false`` | ``bool``

### 2. How do I handle erroneous input?

Cori offers several possibilities for responding to erroneous input. Different functions for each conversion exist based on how much control you want over the error handling.

Input can be erroneous in 2 ways:

- Type error: the issued string cannot be meaningfully converted to the target type without assuming the intent of the user.\
For example, while it may technically be possible to retrieve a single character if the user entered multiple, it's best to cause an error as it's certainly a mistake from the user.\
When the input cannot be converted at all, such as trying to convert ``dfsfds`` into an integer, it's also a type error.
- Logic error : the issued string has been converted successfully, but the resulting value isn't valid according to the logic of the program.\
For example, a file path that does not exist for a text editor.

Cori only implements error handling for type errors.

#### 1. I don't care about erroneous input

**Function name prefix**: ``read_<typename>``

**Behavior on erroneous input**: calls the default input error handler and retries.

Sample code:

```c
printf("How old are you ? ");
int result = read_int();
```

If no default input error handler is specified, the error is simply swallowed. This means the user will get successive prompts with no message between them.

To define a default input error handler, define the ``CORI_DEFAULT_INPUT_ERROR_HANDLER`` macro before including and define the ``void handle_inputError(InputError)`` function.

Sample code:

```c
#define CORI_DEFAULT_INPUT_ERROR_HANDLER
#define CORI_IMPLEMENTATION
#include "stb_cori.h"

void handle_inputError(InputError error)
{
    // ...
}
```

#### 2. I want to call a function on erroneous input and retry

**Function name**: ``read_<typename>_handleErrors``

**Behavior on erroneous input**: calls the specified input error handler and retries.

These functions take a function pointer parameter of type ``InputErrorHandler``, which is equivalent to ``void (*InputErrorHandler)(InputError)``.

Basically, input error handlers take 1 argument, the error that occured, and returns nothing.

``InputError`` is an enumeration representing all possible error conditions. Its values are prefixed with ``IE_``.

Sample code:

```c
printf("How old are you ? ");
int result = read_int_handleErrors(&handle_inputError);
```

Sample input eror handler:

```c
void handle_inputError(InputError error)
{
    char *s = NULL;
    switch (error) {
    case IE_EMPTY: s = "Empty input"; break;
    case IE_EOF: s = "EOF reached"; break;
    case IE_INVALID_BOOLEAN_CHAR: s = "Invalid boolean char"; break;
    case IE_MULTIPLE_CHARS: s = "Multiple chars"; break;
    case IE_NOT_A_NUMBER: s = "Not a number"; break;
    case IE_NUMBER_OUT_OF_BOUNDS: s = "Number out of bounds"; break;
    case IE_OUT_OF_MEMORY: s = "Out of memory"; break;
    case IE_UNSUPPORTED_BASE: s = "Unsupported numeric base"; break;
    case IE_OK: return; // don't print if IE_OK
    }
    puts(s);
}
```

#### 3. I want maximum control: read the input once, and let me do the rest

**Function name**: ``tryRead_<typename>``

**Behavior on erroneous input**: returns the appropriate ``InputError`` value.

These functions return an error code instead of the converted result, meaning that the result variable has to be passed as a pointer argument.

They follow the usual "try-function" pattern (link?) with an out argument.

They are especially useful in loops. Indeed, an ``InputError`` value can be treated as a boolean indicating whether or not an error occured, as:

- ``IE_OK``, the value that indicates that no error occured, has the numeric value ``0``, which means it evaluates to ``false``
- Any other value has a non-zero numeric value, meaning it evaluates to ``true``

Sample code:

```c
InputError error;
int result;
do {
    printf("How old are you ? ");
    error = tryRead_int(&result);
    handle_inputError(error);
} while (error);
printf("You're %d.\n", (int)result);
```

> [!WARNING]
> Since ``tryRead`` functions take a pointer, you must pass a pointer to a type identical to or of the same size as the actual result type.

### 3. Where do I want to read from?

You may want to read from either ``stdin``, or some other stream.

Functions that end with ``_from`` take a ``FILE *`` parameter, the stream to read from.

Other functions read from ``stdin``.

## Reserved identifers

Any identifier starting with ``_cori_`` is defined as private and should not be referenced.
