/*
    Copyright (c) 2023 BB-301 <fw3dg3@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED
#define FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED 1
#endif

#include <ctype.h>
#include <assert.h>
#include <stdarg.h>
#include <regex.h>
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED == 1)
#include <pthread.h>
#endif

#include "fancy_string.h"
#include "fancy_memory.h"

#define __FANCY_STRING_LIB_VERSION_MAJOR__ 0
#define __FANCY_STRING_LIB_VERSION_MINOR__ 1
#define __FANCY_STRING_LIB_VERSION_REVISION__ 0

#define FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED_REQUIRED_ERROR_MESSAGE "This method requires that the library has been built using the 'FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED = 1' preprocessor flag in order to be used."

// -----------------------------------------------
//      PRIVATE HELPERS INTERFACES AND MACROS
// -----------------------------------------------

#define IGNORE_UNUSED(x) ((void)(x))

#define FAIL_WITH_MESSAGE(message)                                               \
    fprintf(stderr, "[libfancy_string failure:%s] %s\n", __FUNCTION__, message); \
    exit(EXIT_FAILURE)

static void *my_malloc(size_t size);

static void my_free(void *ptr);

static void *my_realloc(void *ptr, size_t size);

static char *copy_value(char const *const value);

static bool is_whitespace(char *c);

static void to_lower_or_upper_case(fancy_string_t *const self, bool use_lower);

// -----------------------------------------------
//             OPAQUE TYPES DEFINITIONS
// -----------------------------------------------

struct fancy_string_s
{
    char *value;
    size_t n;
};

struct fancy_string_array_s
{
    fancy_string_t **array;
    size_t n;
};

struct fancy_string_regex_s
{
    fancy_string_t *pattern;
    fancy_string_t *string;
    ssize_t n_max_matches;
    size_t n_matches;
    size_t *start_array;
    size_t *end_array;
};

// -----------------------------------------------
//                      MISC
// -----------------------------------------------

void fancy_string_library_version(uint16_t *major, uint16_t *minor, uint16_t *revision)
{
    *major = __FANCY_STRING_LIB_VERSION_MAJOR__;
    *minor = __FANCY_STRING_LIB_VERSION_MINOR__;
    *revision = __FANCY_STRING_LIB_VERSION_REVISION__;
}

void fancy_string_library_version_print(FILE *stream)
{
    assert(stream != NULL);

    fprintf(
        stream, "lib 'fancy_string' version: %i.%i.%i\n",
        __FANCY_STRING_LIB_VERSION_MAJOR__,
        __FANCY_STRING_LIB_VERSION_MINOR__,
        __FANCY_STRING_LIB_VERSION_REVISION__);
}

// -----------------------------------------------
//                 STRING (methods)
// -----------------------------------------------

fancy_string_t *fancy_string_create(char const *const value)
{
    assert(value != NULL);

    fancy_string_t *self = my_malloc(sizeof(fancy_string_t));
    self->value = copy_value(value);
    self->n = strlen(self->value);
    return self;
}

fancy_string_t *fancy_string_from_stream(FILE *stream)
{
    fancy_string_t *s = fancy_string_create_empty();
    if (stream == NULL)
    {
        return s;
    }
    char *tmp = my_malloc(sizeof(char) * 2);
    tmp[1] = '\0';
    int c;
    // NOTE: [With GNU C Library, EOF is -1](https://www.gnu.org/software/libc/manual/html_node/EOF-and-Errors.html)
    // NOTE 2: Using flockfile + getc_unlocked instead of fgetc
    // - [flockfile](https://www.gnu.org/software/libc/manual/html_node/Streams-and-Threads.html)
    // - [getc_unlocked](https://www.gnu.org/software/libc/manual/html_node/Character-Input.html)
    flockfile(stream);
    // while ((c = fgetc(stream)) != EOF)
    while ((c = getc_unlocked(stream)) != EOF)
    {
        tmp[0] = (char)c;
        fancy_string_append_value(s, tmp);
    }
    funlockfile(stream);
    my_free(tmp);
    fclose(stream);
    return s;
}

fancy_string_t *fancy_string_from_stream_next_line(FILE *stream)
{
    // [main reference](https://man7.org/linux/man-pages/man3/getline.3.html)
    if (stream == NULL)
    {
        return NULL;
    }

    char *line = NULL;
    size_t line_size = 0;

    ssize_t size = getline(&line, &line_size, stream);
    if (size == -1)
    {
        free(line);
        return NULL;
    }

    fancy_string_t *s = fancy_string_create(line);

    free(line);

    fancy_string_replace_value(s, "\n", "", -1);

    return s;
}

fancy_string_t *fancy_string_create_repeat(char const *const value, size_t n_repeat)
{
    assert(value != NULL);

    fancy_string_t *self = fancy_string_create(value);
    if (n_repeat == 0)
    {
        return self;
    }
    fancy_string_t *clone = fancy_string_clone(self);
    size_t i = 0;
    while (i < n_repeat)
    {
        fancy_string_append(self, clone);
        i++;
    }
    fancy_string_destroy(clone);
    return self;
}

fancy_string_t *fancy_string_create_empty(void)
{
    return fancy_string_create("");
}

fancy_string_t *fancy_string_clone(fancy_string_t const *const self)
{
    assert(self != NULL);

    return fancy_string_create(self->value);
}

void fancy_string_destroy(fancy_string_t *const self)
{
    assert(self != NULL);

    my_free(self->value);
    my_free(self);
}

void fancy_string_print(fancy_string_t const *const self, FILE *stream, bool debug)
{
    assert(self != NULL);
    assert(stream != NULL);

    if (debug)
    {
        fprintf(stream, "fancy_string_t[%zu](%s)\n", fancy_string_size(self), self->value);
    }
    else
    {
        fprintf(stream, "%s", self->value);
    }
}

size_t fancy_string_size(fancy_string_t const *const self)
{
    assert(self != NULL);

    return self->n;
}

bool fancy_string_is_empty(fancy_string_t const *const self)
{
    assert(self != NULL);

    return fancy_string_size(self) == 0;
}

void fancy_string_clear(fancy_string_t *const self)
{
    assert(self != NULL);

    my_free(self->value);
    self->value = my_malloc(sizeof(char));
    self->value[0] = '\0';
    self->n = 0;
}

void fancy_string_update_value(fancy_string_t *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    // I don't know why, but using `fancy_string_clear(self)`
    //  here instead of simply `my_free(self->value)` causes a memory
    //  leak each time this method gets called.
    // fancy_string_clear(self);
    // The `fancy_string_clear` call expends to:
    // - my_free(self->value);
    //  - self->value = my_malloc(sizeof(char));
    // - self->value[0] = '\0';
    // - self->n = 0;
    // Obviously, this is wasteful since we will again replace
    // the memory. Maybe the compiler does an optimization, or something, which
    //  result in something funny?

    my_free(self->value);
    self->value = copy_value(value);
    self->n = strlen(self->value);
}

void fancy_string_update(fancy_string_t *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    fancy_string_update_value(self, string->value);
}

char *fancy_string_value(fancy_string_t const *const self)
{
    assert(self != NULL);

    size_t n = fancy_string_size(self);
    char *value = malloc(sizeof(char) * (n + 1));
    memcpy(value, self->value, n);
    value[n] = '\0';
    return value;
}

void fancy_string_append_value(fancy_string_t *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    size_t n_value = strlen(value);
    if (n_value == 0)
    {
        return;
    }
    size_t n = fancy_string_size(self);
    if (n == 0)
    {
        fancy_string_update_value(self, value);
        return;
    }
    self->value = my_realloc(self->value, sizeof(char) * (n + n_value + 1));
    memcpy(&self->value[n], value, n_value);
    self->value[n + n_value] = '\0';
    self->n = strlen(self->value);
}

void fancy_string_append(fancy_string_t *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    fancy_string_append_value(self, string->value);
}

void fancy_string_prepend_value(fancy_string_t *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    size_t n_value = strlen(value);
    if (n_value == 0)
    {
        return;
    }
    size_t n = fancy_string_size(self);
    if (n == 0)
    {
        fancy_string_update_value(self, value);
        return;
    }
    fancy_string_t *tmp = fancy_string_create(value);
    fancy_string_append(tmp, self);
    fancy_string_update(self, tmp);
    fancy_string_destroy(tmp);
}

void fancy_string_prepend(fancy_string_t *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    fancy_string_prepend_value(self, string->value);
}

bool fancy_string_equals_value(fancy_string_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    size_t n_value = strlen(value);
    size_t n = fancy_string_size(self);
    if (n_value != n)
    {
        return false;
    }
    return memcmp(self->value, value, n) == 0;
}

bool fancy_string_equals(fancy_string_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    return fancy_string_equals_value(self, string->value);
}

bool fancy_string_starts_with_value(fancy_string_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    size_t n_value = strlen(value);
    size_t n = fancy_string_size(self);
    if (n_value == 0 && n == 0)
    {
        // NOTE: The rational behind this is that if both values are "empty",
        // then they are equal, and if they are equal, then it makes sense to
        // say that they start with the same value.
        return true;
    }
    if (n_value == 0 || n_value > n)
    {
        return false;
    }
    return memcmp(self->value, value, n_value) == 0;
}

bool fancy_string_starts_with(fancy_string_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    return fancy_string_starts_with_value(self, string->value);
}

bool fancy_string_ends_with_value(fancy_string_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    size_t n_value = strlen(value);
    size_t n = fancy_string_size(self);
    if (n_value == 0 && n == 0)
    {
        // NOTE: The rational behind this is that if both values are "empty",
        // then they are equal, and if they are equal, then it makes sense to
        // say that they end with the same value.
        return true;
    }
    if (n_value == 0 || n_value > n)
    {
        return false;
    }
    return memcmp(&self->value[n - n_value], value, n_value) == 0;
}

bool fancy_string_ends_with(fancy_string_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    return fancy_string_ends_with_value(self, string->value);
}

ssize_t fancy_string_index_of_value(fancy_string_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    size_t n_value = strlen(value);
    size_t n = fancy_string_size(self);
    if (n_value > n)
    {
        return -1;
    }
    else if (n_value == 0 || n == 0)
    {
        return -1;
    }
    char *p = strstr(self->value, value);
    if (p == NULL)
    {
        return -1;
    }
    return (ssize_t)(p - self->value);
}

ssize_t fancy_string_index_of(fancy_string_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    return fancy_string_index_of_value(self, string->value);
}

bool fancy_string_contains_value(fancy_string_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    return fancy_string_index_of_value(self, value) != -1;
}

bool fancy_string_contains(fancy_string_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    return fancy_string_index_of(self, string) != -1;
}

fancy_string_t *fancy_string_substring(fancy_string_t const *const self, ssize_t start, ssize_t end)
{
    assert(self != NULL);

    size_t n = fancy_string_size(self);
    if (n == 0)
    {
        return fancy_string_create_empty();
    }

    size_t index_start = start == -1 ? 0 : (size_t)start;
    size_t index_end = end == -1 ? n : (size_t)end;

    if (index_start == index_end)
    {
        return fancy_string_create_empty();
    }

    if (index_start > index_end)
    {
        size_t index_tmp = index_start;
        index_start = index_end;
        index_end = index_tmp;
    }

    size_t substring_size = index_end - index_start;
    if (n < substring_size)
    {
        if (index_start >= n)
        {
            return fancy_string_create_empty();
        }
        substring_size = n - index_start;
    }

    char *value = self->value;
    value += index_start;
    char *substring = my_malloc(sizeof(char) * (substring_size + 1));
    memcpy(substring, value, substring_size);
    substring[substring_size] = '\0';
    fancy_string_t *out = fancy_string_create(substring);
    my_free(substring);
    return out;
}

void fancy_string_trim_left(fancy_string_t *const self)
{
    assert(self != NULL);

    size_t n = fancy_string_size(self);
    if (n == 0)
    {
        return;
    }
    size_t i = 0;
    while (is_whitespace(&self->value[i]))
    {
        i++;
    }
    if (i == 0)
    {
        return;
    }
    fancy_string_t *substring = fancy_string_substring(self, i, -1);
    fancy_string_update(self, substring);
    fancy_string_destroy(substring);
}

fancy_string_t *fancy_string_trimmed_left(fancy_string_t const *const self)
{
    assert(self != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_trim_left(clone);
    return clone;
}

void fancy_string_trim_right(fancy_string_t *const self)
{
    assert(self != NULL);

    size_t n = fancy_string_size(self);
    if (n == 0)
    {
        return;
    }
    size_t i = n - 1;
    while (is_whitespace(&self->value[i]))
    {
        if (i == 0)
        {
            break;
        }
        i--;
    }
    if (i == n - 1)
    {
        return;
    }
    fancy_string_t *substring = fancy_string_substring(self, 0, i + 1);
    fancy_string_update(self, substring);
    fancy_string_destroy(substring);
}

fancy_string_t *fancy_string_trimmed_right(fancy_string_t const *const self)
{
    assert(self != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_trim_right(clone);
    return clone;
}

void fancy_string_trim(fancy_string_t *const self)
{
    assert(self != NULL);

    fancy_string_trim_left(self);
    fancy_string_trim_right(self);
}

fancy_string_t *fancy_string_trimmed(fancy_string_t const *const self)
{
    assert(self != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_trim(clone);
    return clone;
}

fancy_string_array_t *fancy_string_split_by_value(fancy_string_t const *const self, char const *const separator, ssize_t n_max_splits)
{
    assert(self != NULL);
    assert(separator != NULL);

    if (n_max_splits == 0)
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push(a, self);
        return a;
    }

    fancy_string_array_t *array = fancy_string_array_create();
    fancy_string_t *remainder = fancy_string_clone(self);
    size_t n_separator = strlen(separator);

    size_t split_counter = 0;

    while (true)
    {
        if (n_max_splits != -1 && (size_t)n_max_splits <= split_counter)
        {
            fancy_string_array_push(array, remainder);
            fancy_string_destroy(remainder);
            break;
        }

        split_counter += 1;

        ssize_t index = fancy_string_index_of_value(remainder, separator);
        if (index == -1)
        {
            fancy_string_array_push(array, remainder);
            fancy_string_destroy(remainder);
            break;
        }
        else
        {
            fancy_string_t *substring = fancy_string_substring(remainder, 0, index);
            fancy_string_array_push(array, substring);
            fancy_string_destroy(substring);

            fancy_string_t *new_remainder = fancy_string_substring(remainder, index + n_separator, -1);
            fancy_string_destroy(remainder);
            remainder = new_remainder;
        }
    }

    return array;
}

fancy_string_array_t *fancy_string_split(fancy_string_t const *const self, fancy_string_t const *const separator, ssize_t n_max_splits)
{
    assert(self != NULL);
    assert(separator != NULL);

    return fancy_string_split_by_value(self, separator->value, n_max_splits);
}

void fancy_string_pad_start(fancy_string_t *const self, size_t target_size, char value)
{
    assert(self != NULL);

    size_t n = fancy_string_size(self);
    if (n >= target_size)
    {
        return;
    }
    char *tmp_value = my_malloc(sizeof(char) * 2);
    tmp_value[0] = value;
    tmp_value[1] = '\0';
    fancy_string_t *tmp = fancy_string_create_repeat(tmp_value, target_size - n - 1);
    my_free(tmp_value);
    fancy_string_prepend(self, tmp);
    fancy_string_destroy(tmp);
}

fancy_string_t *fancy_string_padded_start(fancy_string_t const *const self, size_t target_size, char value)
{
    assert(self != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_pad_start(clone, target_size, value);
    return clone;
}

void fancy_string_pad_end(fancy_string_t *const self, size_t target_size, char value)
{
    assert(self != NULL);

    size_t n = fancy_string_size(self);
    if (n >= target_size)
    {
        return;
    }
    char *tmp_value = my_malloc(sizeof(char) * 2);
    tmp_value[0] = value;
    tmp_value[1] = '\0';
    fancy_string_t *tmp = fancy_string_create_repeat(tmp_value, target_size - n - 1);
    my_free(tmp_value);
    fancy_string_append(self, tmp);
    fancy_string_destroy(tmp);
}

fancy_string_t *fancy_string_padded_end(fancy_string_t const *const self, size_t target_size, char value)
{
    assert(self != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_pad_end(clone, target_size, value);
    return clone;
}

void fancy_string_replace_value(fancy_string_t *const self, char const *const old_value, char const *const new_value, ssize_t replace_n)
{
    assert(self != NULL);
    assert(old_value != NULL);
    assert(new_value != NULL);

    if (replace_n == 0)
    {
        return;
    }

    size_t n_old_value = strlen(old_value);
    if (n_old_value == 0)
    {
        return;
    }

    fancy_string_t *tmp = fancy_string_clone(self);
    size_t index_offset = 0;

    size_t replace_count = 0;
    while (replace_n == -1 || (size_t)replace_n > replace_count)
    {
        ssize_t index = fancy_string_index_of_value(tmp, old_value);
        if (index == -1)
        {
            break;
        }

        fancy_string_t *prefix = fancy_string_substring(self, 0, index + index_offset);
        fancy_string_t *suffix = fancy_string_substring(self, index + index_offset + n_old_value, -1);

        fancy_string_append_value(prefix, new_value);
        index_offset = fancy_string_size(prefix);

        fancy_string_append(prefix, suffix);

        fancy_string_update(self, prefix);

        fancy_string_destroy(prefix);
        fancy_string_destroy(tmp);

        tmp = suffix;
        replace_count++;
    }

    fancy_string_destroy(tmp);
}

void fancy_string_replace(fancy_string_t *const self, fancy_string_t const *const old_substring, fancy_string_t const *const new_substring, ssize_t replace_n)
{
    assert(self != NULL);
    assert(old_substring != NULL);
    assert(new_substring != NULL);

    fancy_string_replace_value(self, old_substring->value, new_substring->value, replace_n);
}

fancy_string_t *fancy_string_replaced_value(fancy_string_t const *const self, char const *const old_value, char const *const new_value, ssize_t replace_n)
{
    assert(self != NULL);
    assert(old_value != NULL);
    assert(new_value != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_replace_value(clone, old_value, new_value, replace_n);
    return clone;
}

fancy_string_t *fancy_string_replaced(fancy_string_t const *const self, fancy_string_t const *const old_substring, fancy_string_t const *const new_substring, ssize_t replace_n)
{
    assert(self != NULL);
    assert(old_substring != NULL);
    assert(new_substring != NULL);

    return fancy_string_replaced_value(self, old_substring->value, new_substring->value, replace_n);
}

void fancy_string_lowercase(fancy_string_t *const self)
{
    assert(self != NULL);

    to_lower_or_upper_case(self, true);
}

fancy_string_t *fancy_string_lowercased(fancy_string_t const *const self)
{
    assert(self != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_lowercase(clone);
    return clone;
}

void fancy_string_uppercase(fancy_string_t *const self)
{
    assert(self != NULL);

    to_lower_or_upper_case(self, false);
}

fancy_string_t *fancy_string_uppercased(fancy_string_t const *const self)
{
    assert(self != NULL);

    fancy_string_t *clone = fancy_string_clone(self);
    fancy_string_uppercase(clone);
    return clone;
}

fancy_string_regex_t *fancy_string_regex_create(fancy_string_t const *const string, fancy_string_t const *const pattern, ssize_t n_max_matches)
{
    assert(string != NULL);
    assert(pattern != NULL);

    if (fancy_string_is_empty(pattern))
    {
        return NULL;
    }

    size_t n_string = fancy_string_size(string);
    if (n_string == 0)
    {
        return NULL;
    }

    regex_t re;
    int reason_code;

    fancy_string_t *regular_expression_clone = fancy_string_clone(pattern);
    // NOTE: Here we use the internal clone to avoid relying on an external value (i.e., we want
    // `re` to use memory that belongs (in this case, will belong) to `self`.
    reason_code = regcomp(&re, regular_expression_clone->value, REG_EXTENDED);
    if (reason_code != 0)
    {
        // NOTE: `regcomp` does not appear to be using `errno` for error reporting. My
        // understanding is that it returns the error code directly (see `man 3 regcomp`).
        // So we don't need to bother about resetting `errno` if we encounter and error. For
        // now, the API simply returns NULL if an error occurs.
        regfree(&re);
        fancy_string_destroy(regular_expression_clone);
        return NULL;
    }

    fancy_string_regex_t *self = my_malloc(sizeof(fancy_string_regex_t));

    self->pattern = regular_expression_clone;
    self->string = fancy_string_clone(string);
    self->n_max_matches = n_max_matches;
    self->n_matches = 0;
    self->start_array = NULL;
    self->end_array = NULL;

    char *tmp = self->string->value;
    while (self->n_max_matches == -1 || self->n_matches < (size_t)self->n_max_matches)
    {
        regmatch_t match;
        reason_code = regexec(&re, tmp, 1, &match, 0);
        if (reason_code == 0)
        {
            if (
                ((unsigned)match.rm_so) > n_string ||
                ((unsigned)match.rm_eo) > n_string ||
                match.rm_so > match.rm_eo)
            {
                // fprintf(stderr, "We probably have garbage. Please review this\n");
                regfree(&re);
                fancy_string_regex_destroy(self);
                return NULL;
            }

            if (match.rm_so == match.rm_eo)
            {
                // We need this, else the loop will go on forever
                // in situations where we have a "wildcard match".
                break;
            }

            if (self->start_array == NULL)
            {
                self->start_array = my_malloc(sizeof(size_t));
                self->end_array = my_malloc(sizeof(size_t));
            }
            else
            {
                self->start_array = my_realloc(self->start_array, sizeof(size_t) * (self->n_matches + 1));
                self->end_array = my_realloc(self->end_array, sizeof(size_t) * (self->n_matches + 1));
            }
            size_t offset = self->n_matches > 0 ? self->end_array[self->n_matches - 1] : 0;
            self->start_array[self->n_matches] = match.rm_so + offset;
            self->end_array[self->n_matches] = match.rm_eo + offset;
            self->n_matches += 1;
            tmp += match.rm_eo;
        }
        else if (reason_code == REG_NOMATCH)
        {
            break;
        }
        else
        {
            regfree(&re);
            fancy_string_regex_destroy(self);
            return NULL;
        }
    }

    regfree(&re);

    return self;
}

void fancy_string_regex_destroy(fancy_string_regex_t *const self)
{
    assert(self != NULL);

    if (self->start_array != NULL)
    {
        my_free(self->start_array);
    }
    if (self->end_array != NULL)
    {
        my_free(self->end_array);
    }
    fancy_string_destroy(self->pattern);
    fancy_string_destroy(self->string);
    my_free(self);
}

static void fancy_string_regex_debug_verbose_callback(fancy_string_t *const match, size_t start, size_t end, fancy_string_t const *const string, void *context)
{
    IGNORE_UNUSED(start);
    IGNORE_UNUSED(end);
    IGNORE_UNUSED(string);
    IGNORE_UNUSED(context);

    fancy_string_prepend_value(match, "{{");
    fancy_string_append_value(match, "}}");
}

void fancy_string_regex_debug(fancy_string_regex_t const *const self, FILE *stream, bool verbose)
{
    assert(self != NULL);

    fancy_string_array_t *matches = NULL;
    fancy_string_t *verbose_string = NULL;

    if (verbose)
    {
        matches = fancy_string_regex_matches_to_strings(self);
        verbose_string = fancy_string_regex_to_string_with_updated_matches(self, fancy_string_regex_debug_verbose_callback, NULL);
    }

    fprintf(stream, "fancy_string_regex_t {\n");
    fprintf(stream, "\t.string = fancy_string_t[%zu](%s),\n", self->string->n, self->string->value);
    fprintf(stream, "\t.pattern = fancy_string_t[%zu](%s),\n", self->pattern->n, self->pattern->value);
    fprintf(stream, "\t.n_matches = %zu,\n", self->n_matches);
    fprintf(stream, "\t.n_max_matches = %zd,\n", self->n_max_matches);
    fprintf(stream, "\t.matches = {\n");
    for (size_t i = 0; i < self->n_matches; i++)
    {
        if (matches != NULL)
        {
            fprintf(
                stream,
                "\t\t { .start = %zu, .end = %zu, .verbose_match[calculated] = fancy_string_t[%zu](%s) }\n",
                self->start_array[i],
                self->end_array[i],
                matches->array[i]->n,
                matches->array[i]->value);
        }
        else
        {
            fprintf(stream, "\t\t { .start = %zu, .end = %zu }\n", self->start_array[i], self->end_array[i]);
        }
    }
    fprintf(stream, "\t},\n");
    if (verbose_string != NULL)
    {
        fprintf(stream, "\t.verbose_string[calculated] = fancy_string_t[%zu](%s),\n", verbose_string->n, verbose_string->value);
    }
    fprintf(stream, "}\n");

    if (matches != NULL)
    {
        fancy_string_array_destroy(matches);
        fancy_string_destroy(verbose_string);
    }
}

fancy_string_array_t *fancy_string_regex_matches_to_strings(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    fancy_string_array_t *a = fancy_string_array_create();

    for (size_t i = 0; i < self->n_matches; i++)
    {
        size_t start = self->start_array[i];
        size_t end = self->end_array[i];
        fancy_string_t *s = fancy_string_substring(self->string, start, end);
        fancy_string_array_push(a, s);
        fancy_string_destroy(s);
    }

    return a;
}

fancy_string_t *fancy_string_regex_replaced_matches(fancy_string_regex_t *const self, fancy_string_t *new_string)
{
    assert(self != NULL);
    assert(new_string != NULL);

    if (self->n_matches == 0)
    {
        return fancy_string_clone(self->string);
    }

    fancy_string_t *string = fancy_string_create_empty();

    size_t previous_end = 0;
    for (size_t i = 0; i < self->n_matches; i++)
    {
        fancy_string_t *prefix = fancy_string_substring(self->string, previous_end, self->start_array[i]);
        fancy_string_append(string, prefix);
        fancy_string_destroy(prefix);
        fancy_string_append(string, new_string);
        previous_end = self->end_array[i];
    }

    fancy_string_t *tmp = fancy_string_substring(self->string, previous_end, -1);
    fancy_string_append(string, tmp);
    fancy_string_destroy(tmp);

    return string;
}

fancy_string_array_t *fancy_string_regex_split_at_matches(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    if (self->n_matches == 0)
    {
        return fancy_string_array_create_with_values(self->string->value, NULL);
    }

    fancy_string_array_t *array = fancy_string_array_create();

    size_t previous_end = 0;
    for (size_t i = 0; i < self->n_matches; i++)
    {
        fancy_string_t *tmp = fancy_string_substring(self->string, previous_end, self->start_array[i]);
        fancy_string_array_push(array, tmp);
        fancy_string_destroy(tmp);
        previous_end = self->end_array[i];
    }

    fancy_string_t *tmp = fancy_string_substring(self->string, previous_end, -1);
    // NOTE: We add it anyways, else it won't be consistent with `fancy_string_split`
    // if (!fancy_string_is_empty(tmp))
    // {
    //     fancy_string_array_push(array, tmp);
    // }
    fancy_string_array_push(array, tmp);
    fancy_string_destroy(tmp);

    return array;
}

size_t fancy_string_regex_match_count(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    return self->n_matches;
}

bool fancy_string_regex_has_match(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    return self->n_matches > 0 ? true : false;
}

ssize_t fancy_string_regex_max_number_of_matches(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    return self->n_max_matches;
}

bool fancy_string_regex_max_number_of_matches_reached(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    if (self->n_max_matches == -1)
    {
        return false;
    }

    return self->n_matches == ((size_t)self->n_max_matches) ? true : false;
}

fancy_string_t *fancy_string_regex_string(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    return fancy_string_clone(self->string);
}

fancy_string_t *fancy_string_regex_pattern(fancy_string_regex_t const *const self)
{
    assert(self != NULL);

    return fancy_string_clone(self->pattern);
}

fancy_string_t *fancy_string_regex_to_string_with_updated_matches(fancy_string_regex_t const *const self, fancy_string_regex_updater_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    if (self->n_matches == 0)
    {
        return fancy_string_clone(self->string);
    }

    fancy_string_t *updated_string = fancy_string_create_empty();

    fancy_string_array_t *unmatched_components = fancy_string_regex_split_at_matches(self);
    fancy_string_array_t *matched_components = fancy_string_regex_matches_to_strings(self);

    for (size_t i = 0; i < self->n_matches; i++)
    {
        fancy_string_t *match = fancy_string_array_shift(matched_components);
        size_t start = self->start_array[i];
        size_t end = self->end_array[i];
        //  This call (potentially) updates "match", which we then use to build the new string.
        (*fn)(match, start, end, self->string, context);

        if (i == 0 && start > 0)
        {
            fancy_string_t *unmatched = fancy_string_array_shift(unmatched_components);
            if (unmatched != NULL)
            {
                fancy_string_append(updated_string, unmatched);
                fancy_string_destroy(unmatched);
            }
            fancy_string_append(updated_string, match);
        }
        else if (i == 0 && start == 0)
        {
            fancy_string_append(updated_string, match);
            fancy_string_t *unmatched = fancy_string_array_shift(unmatched_components);
            if (unmatched != NULL)
            {
                fancy_string_append(updated_string, unmatched);
                fancy_string_destroy(unmatched);
            }
        }
        else
        {
            fancy_string_t *unmatched = fancy_string_array_shift(unmatched_components);
            if (unmatched != NULL)
            {
                fancy_string_append(updated_string, unmatched);
                fancy_string_destroy(unmatched);
            }
            fancy_string_append(updated_string, match);
        }

        fancy_string_destroy(match);
    }

    fancy_string_t *unmatched = fancy_string_array_shift(unmatched_components);
    if (unmatched != NULL)
    {
        fancy_string_append(updated_string, unmatched);
        fancy_string_destroy(unmatched);
    }

    fancy_string_array_destroy(unmatched_components);
    fancy_string_array_destroy(matched_components);

    return updated_string;
}

fancy_string_regex_match_info_t fancy_string_regex_match_info_for_index(fancy_string_regex_t const *const self, size_t index)
{
    assert(self != NULL);

    fancy_string_regex_match_info_t info = {.index = -1, .start = 0, .end = 0};

    if (self->n_matches <= index)
    {
        return info;
    }

    info.index = (ssize_t)index;
    info.start = self->start_array[index];
    info.end = self->end_array[index];

    return info;
}

// -----------------------------------------------
//                      ARRAY
// -----------------------------------------------

fancy_string_array_t *fancy_string_array_create(void)
{
    fancy_string_array_t *self = my_malloc(sizeof(fancy_string_array_t));
    self->n = 0;
    self->array = NULL;
    return self;
}

fancy_string_array_t *fancy_string_array_create_with_values(char const *const first_value, ...)
{
    fancy_string_array_t *a = fancy_string_array_create();

    if (first_value == NULL)
    {
        return a;
    }

    va_list args;
    va_start(args, first_value);

    fancy_string_array_push_value(a, first_value);

    while (true)
    {
        char *tmp = va_arg(args, char *);
        if (tmp == NULL)
        {
            break;
        }
        fancy_string_array_push_value(a, tmp);
    }

    va_end(args);

    return a;
}

fancy_string_array_t *fancy_string_array_clone(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    fancy_string_array_t *clone = fancy_string_array_create();

    for (size_t i = 0; i < self->n; i++)
    {
        fancy_string_array_push(clone, self->array[i]);
    }

    return clone;
}

void fancy_string_array_append(fancy_string_array_t *const self, fancy_string_array_t const *const array)
{
    assert(self != NULL);
    assert(array != NULL);

    for (size_t i = 0; i < array->n; i++)
    {
        fancy_string_array_push(self, array->array[i]);
    }
}

void fancy_string_array_append_and_destroy(fancy_string_array_t *const self, fancy_string_array_t *array)
{
    assert(self != NULL);
    assert(array != NULL);

    fancy_string_array_append(self, array);
    fancy_string_array_destroy(array);
}

void fancy_string_array_destroy(fancy_string_array_t *const self)
{
    assert(self != NULL);

    fancy_string_array_clear(self);
    my_free(self);
}

void fancy_string_array_print(fancy_string_array_t const *const self, FILE *stream, bool debug)
{
    assert(self != NULL);
    assert(stream != NULL);

    if (debug)
    {
        fprintf(stream, "fancy_string_array_t[%zu] {\n", self->n);
        for (size_t i = 0; i < self->n; i++)
        {
            fprintf(
                stream, "\tfancy_string_t[%zu](%s),\n",
                fancy_string_size(self->array[i]),
                self->array[i]->value);
        }
        fprintf(stream, "}\n");
    }
    else
    {
        for (size_t i = 0; i < self->n; i++)
        {
            fancy_string_print(self->array[i], stream, false);
        }
    }
}

void fancy_string_array_clear(fancy_string_array_t *const self)
{
    assert(self != NULL);

    while (true)
    {
        if (self->n == 0)
        {
            break;
        }

        fancy_string_destroy(self->array[self->n - 1]);
        if (self->n == 1)
        {
            my_free(self->array);
        }
        else
        {
            self->array = my_realloc(self->array, sizeof(fancy_string_t *) * (self->n - 1));
        }

        self->n--;
    }
}

size_t fancy_string_array_size(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    return self->n;
}

bool fancy_string_array_is_empty(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    return fancy_string_array_size(self) == 0;
}

void fancy_string_array_push_value(fancy_string_array_t *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    fancy_string_t *string = fancy_string_create(value);
    if (self->n == 0)
    {
        self->array = my_malloc(sizeof(fancy_string_t *));
    }
    else
    {
        self->array = my_realloc(self->array, sizeof(fancy_string_t *) * (self->n + 1));
    }
    self->array[self->n] = string;
    self->n++;
}

void fancy_string_array_push_values(fancy_string_array_t *const self, char const *const first_value, ...)
{
    assert(self != NULL);

    if (first_value == NULL)
    {
        return;
    }

    fancy_string_array_t *a = fancy_string_array_create();

    va_list args;
    va_start(args, first_value);

    fancy_string_array_push_value(a, first_value);

    while (true)
    {
        char *tmp = va_arg(args, char *);
        if (tmp == NULL)
        {
            break;
        }
        fancy_string_array_push_value(a, tmp);
    }

    va_end(args);

    fancy_string_array_append_and_destroy(self, a);
}

void fancy_string_array_push(fancy_string_array_t *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    fancy_string_array_push_value(self, string->value);
}

fancy_string_t *fancy_string_array_get(fancy_string_array_t const *const self, size_t index)
{
    assert(self != NULL);

    if (self->n <= index)
    {
        return NULL;
    }

    return fancy_string_clone(self->array[index]);
}

char *fancy_string_array_get_value(fancy_string_array_t const *const self, size_t index)
{
    assert(self != NULL);

    fancy_string_t *string = fancy_string_array_get(self, index);
    if (string == NULL)
    {
        return NULL;
    }
    char *value = fancy_string_value(string);
    fancy_string_destroy(string);
    return value;
}

fancy_string_t *fancy_string_array_remove(fancy_string_array_t *const self, size_t index)
{
    assert(self != NULL);

    if (self->n <= index)
    {
        return NULL;
    }

    // NOTE: Without this block, the code below (i.e., after this block)
    // introduces a small leak. [This thread](https://stackoverflow.com/questions/16759849/using-realloc-x-0-instead-of-free-and-using-malloc-with-length-of-a-string)
    //  suggests that calling `realloc` with size zero will not result in
    // and internal call to `free`, depending on the implementation.
    if (self->n == 1)
    {
        fancy_string_t *tmp = self->array[0];
        my_free(self->array);
        self->array = NULL;
        self->n = 0;
        return tmp;
    }

    fancy_string_t *string = self->array[index];

    for (size_t i = index; i < self->n - 1; i++)
    {
        self->array[i] = self->array[i + 1];
    }
    self->n -= 1;
    self->array = my_realloc(self->array, sizeof(fancy_string_t *) * self->n);

    return string;
}

void fancy_string_array_remove_and_destroy(fancy_string_array_t *const self, size_t index)
{
    assert(self != NULL);

    fancy_string_t *removed = fancy_string_array_remove(self, index);
    if (removed != NULL)
    {
        fancy_string_destroy(removed);
    }
}

fancy_string_t *fancy_string_array_pop(fancy_string_array_t *const self)
{
    assert(self != NULL);

    if (self->n == 0)
    {
        return NULL;
    }

    return fancy_string_array_remove(self, self->n - 1);
}

void fancy_string_array_pop_and_destroy(fancy_string_array_t *const self)
{
    assert(self != NULL);

    fancy_string_t *string = fancy_string_array_pop(self);
    if (string != NULL)
    {
        fancy_string_destroy(string);
    }
}

fancy_string_t *fancy_string_array_shift(fancy_string_array_t *const self)
{
    assert(self != NULL);

    if (self->n == 0)
    {
        return NULL;
    }

    return fancy_string_array_remove(self, 0);
}

void fancy_string_array_shift_and_destroy(fancy_string_array_t *const self)
{
    assert(self != NULL);

    fancy_string_t *string = fancy_string_array_shift(self);
    if (string != NULL)
    {
        fancy_string_destroy(string);
    }
}

void fancy_string_array_insert_value(fancy_string_array_t *const self, char const *const value, size_t index)
{
    assert(self != NULL);
    assert(value != NULL);

    if (index >= self->n)
    {
        while (self->n < index)
        {
            fancy_string_array_push_value(self, "");
        }
        fancy_string_array_push_value(self, value);
        return;
    }

    self->array = my_realloc(self->array, sizeof(fancy_string_t *) * (self->n + 1));

    for (size_t i = self->n; i > index; i--)
    {
        self->array[i] = self->array[i - 1];
    }
    self->array[index] = fancy_string_create(value);
    self->n += 1;
}

void fancy_string_array_insert(fancy_string_array_t *const self, fancy_string_t const *const string, size_t index)
{
    assert(self != NULL);
    assert(string != NULL);

    fancy_string_array_insert_value(self, string->value, index);
}

fancy_string_t *fancy_string_array_first(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    if (self->n == 0)
    {
        return NULL;
    }

    return fancy_string_clone(self->array[0]);
}

char *fancy_string_array_first_value(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    if (self->n == 0)
    {
        return NULL;
    }

    return fancy_string_value(self->array[0]);
}

fancy_string_t *fancy_string_array_last(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    if (self->n == 0)
    {
        return NULL;
    }

    return fancy_string_clone(self->array[self->n - 1]);
}

char *fancy_string_array_last_value(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    if (self->n == 0)
    {
        return NULL;
    }

    return fancy_string_value(self->array[self->n - 1]);
}

fancy_string_t *fancy_string_array_join_by_value(fancy_string_array_t const *const self, char const *const separator)
{
    assert(self != NULL);
    assert(separator != NULL);

    fancy_string_t *string = fancy_string_create_empty();

    if (self->n == 0)
    {
        return string;
    }

    for (size_t i = 0; i < self->n; i++)
    {
        if (i > 0)
        {
            fancy_string_append_value(string, separator);
        }
        fancy_string_append(string, self->array[i]);
    }

    return string;
}

fancy_string_t *fancy_string_array_join(fancy_string_array_t const *const self, fancy_string_t const *const separator)
{
    assert(self != NULL);
    assert(separator != NULL);

    return fancy_string_array_join_by_value(self, separator->value);
}

void fancy_string_array_reverse(fancy_string_array_t *const self)
{
    assert(self != NULL);

    if (self->n <= 1)
    {
        return;
    }

    fancy_string_t *tmp = NULL;

    size_t bound = (self->n % 2) == 0 ? self->n / 2 : (self->n - 1) / 2;
    for (size_t i = 0; i < bound; i++)
    {
        tmp = self->array[i];
        self->array[i] = self->array[self->n - i - 1];
        self->array[self->n - i - 1] = tmp;
    }
}

fancy_string_array_t *fancy_string_array_reversed(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    fancy_string_array_t *clone = fancy_string_array_clone(self);
    fancy_string_array_reverse(clone);
    return clone;
}

void fancy_string_array_for_each(fancy_string_array_t *const self, fancy_string_for_each_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    for (size_t i = 0; i < self->n; i++)
    {
        fn(self->array[i], i, self, context);
    }
}

fancy_string_array_t *fancy_string_array_mapped(fancy_string_array_t const *const self, fancy_string_mapped_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    fancy_string_array_t *array = fancy_string_array_create();

    for (size_t i = 0; i < self->n; i++)
    {
        fancy_string_t *tmp = fn(self->array[i], i, self, context);
        fancy_string_array_push(array, tmp);
        fancy_string_destroy(tmp);
    }

    return array;
}

static bool simple_array_sorter(fancy_string_t const *const string_1, fancy_string_t const *const string_2, void *context)
{
    IGNORE_UNUSED(context);

    size_t n_1 = fancy_string_size(string_1);
    size_t n_2 = fancy_string_size(string_2);

    size_t n = n_1 > n_2 ? n_2 : n_1;

    for (size_t i = 0; i < n; i++)
    {
        if (string_1->value[i] == string_2->value[i])
        {
            continue;
        }

        bool out = string_1->value[i] < string_2->value[i];
        return out;
    }

    if (n_1 == n_2)
    {
        return true;
    }
    else if (n_2 < n_1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void fancy_string_array_sort(fancy_string_array_t *const self, fancy_string_sort_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    if (self->n <= 1)
    {
        return;
    }

    fancy_string_t *tmp = NULL;

    while (true)
    {
        for (size_t i = 0; i < self->n; i++)
        {
            for (size_t j = self->n - 1; j > i; j--)
            {
                if (!fn(self->array[i], self->array[j], context))
                {
                    tmp = self->array[i];
                    self->array[i] = self->array[j];
                    self->array[j] = tmp;
                    i -= 1;
                    break;
                }
            }
        }
        break;
    }
}

fancy_string_array_t *fancy_string_array_sorted(fancy_string_array_t const *const self, fancy_string_sort_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    fancy_string_array_t *clone = fancy_string_array_clone(self);
    fancy_string_array_sort(clone, fn, context);
    return clone;
}

void fancy_string_array_sort_values(fancy_string_array_t *const self)
{
    assert(self != NULL);

    fancy_string_array_sort(self, simple_array_sorter, NULL);
}

fancy_string_array_t *fancy_string_array_sorted_values(fancy_string_array_t const *const self)
{
    assert(self != NULL);

    return fancy_string_array_sorted(self, simple_array_sorter, NULL);
}

void fancy_string_array_filter(fancy_string_array_t *const self, fancy_string_filter_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    if (self->n == 0)
    {
        return;
    }

    for (size_t i = self->n; i > 0; i--)
    {
        if (!fn(self->array[i - 1], i, self, context))
        {
            fancy_string_array_remove_and_destroy(self, i - 1);
        }
    }
}

fancy_string_array_t *fancy_string_array_filtered(fancy_string_array_t const *const self, fancy_string_filter_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    fancy_string_array_t *clone = fancy_string_array_clone(self);
    fancy_string_array_filter(clone, fn, context);
    return clone;
}

fancy_string_array_t *fancy_string_array_sliced(fancy_string_array_t const *const self, ssize_t start, ssize_t end)
{
    assert(self != NULL);

    size_t n = fancy_string_array_size(self);
    size_t index_start = start == -1 ? 0 : start;
    size_t index_end = end == -1 ? n : end;

    fancy_string_array_t *sliced_array = fancy_string_array_create();

    if (
        index_start >= index_end ||
        index_start >= n)
    {
        return sliced_array;
    }

    if (index_end > n)
    {
        index_end = n;
    }

    if (index_start >= index_end) //  NOTE: We check again because `index_end` might have been modified.
    {
        return sliced_array;
    }

    for (size_t i = index_start; i < index_end; i++)
    {
        fancy_string_array_push(sliced_array, self->array[i]);
    }

    return sliced_array;
}

void fancy_string_array_slice_and_destroy(fancy_string_array_t *const self, ssize_t start, ssize_t end)
{
    assert(self != NULL);

    fancy_string_array_t *slice = fancy_string_array_sliced(self, start, end);
    fancy_string_array_clear(self);
    fancy_string_array_append_and_destroy(self, slice);
}

fancy_string_array_t *fancy_string_array_splice(fancy_string_array_t *const self, size_t index, ssize_t delete_count, fancy_string_array_t const *const optional_new_strings)
{
    assert(self != NULL);

    size_t n = fancy_string_array_size(self);
    size_t positive_delete_count = delete_count == -1 ? n - index : delete_count;

    fancy_string_array_t *removed_strings = fancy_string_array_create();

    for (size_t i = index; (i < index + positive_delete_count) && (i < n); i++)
    {
        fancy_string_t *tmp = fancy_string_array_remove(self, index);
        fancy_string_array_push(removed_strings, tmp);
        fancy_string_destroy(tmp);
    }

    if (optional_new_strings != NULL)
    {
        for (size_t i = 0; i < optional_new_strings->n; i++)
        {
            fancy_string_array_insert(self, optional_new_strings->array[i], index + i);
        }
    }

    return removed_strings;
}

void fancy_string_array_splice_and_destroy(fancy_string_array_t *const self, size_t index, ssize_t delete_count, fancy_string_array_t const *const optional_new_strings)
{
    assert(self != NULL);

    fancy_string_array_t *removed_strings = fancy_string_array_splice(self, index, delete_count, optional_new_strings);
    fancy_string_array_destroy(removed_strings);
}

fancy_string_array_t *fancy_string_array_spliced(fancy_string_array_t const *const self, size_t index, ssize_t delete_count, fancy_string_array_t const *const optional_new_strings)
{
    assert(self != NULL);

    fancy_string_array_t *clone = fancy_string_array_clone(self);
    fancy_string_array_splice_and_destroy(clone, index, delete_count, optional_new_strings);
    return clone;
}

ssize_t fancy_string_array_find_index(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    for (size_t i = 0; i < self->n; i++)
    {
        if (fn(self->array[i], i, self, context))
        {
            return i;
        }
    }
    return -1;
}

fancy_string_t *fancy_string_array_find(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    ssize_t index = fancy_string_array_find_index(self, fn, context);
    if (index == -1)
    {
        return NULL;
    }
    return fancy_string_array_get(self, (size_t)index);
}

ssize_t fancy_string_array_find_last_index(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    for (size_t i = self->n; i > 0; i--)
    {
        size_t index = i - 1;
        if (fn(self->array[index], index, self, context))
        {
            return index;
        }
    }
    return -1;
}

fancy_string_t *fancy_string_array_find_last(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    ssize_t index = fancy_string_array_find_last_index(self, fn, context);
    if (index == -1)
    {
        return NULL;
    }
    return fancy_string_array_get(self, (size_t)index);
}

bool fancy_string_array_some(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    return fancy_string_array_find_index(self, fn, context) != -1;
}

bool fancy_string_array_none(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    return !fancy_string_array_some(self, fn, context);
}

bool fancy_string_array_every(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context)
{
    assert(self != NULL);
    assert(fn != NULL);

    for (size_t i = 0; i < self->n; i++)
    {
        if (!fn(self->array[i], i, self, context))
        {
            return false;
        }
    }
    return true;
}

ssize_t fancy_string_array_index_of(fancy_string_array_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    for (size_t i = 0; i < self->n; i++)
    {
        if (fancy_string_equals(self->array[i], string))
        {
            return i;
        }
    }
    return -1;
}

ssize_t fancy_string_array_index_of_value(fancy_string_array_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    for (size_t i = 0; i < self->n; i++)
    {
        if (fancy_string_equals_value(self->array[i], value))
        {
            return i;
        }
    }
    return -1;
}

ssize_t fancy_string_array_last_index_of(fancy_string_array_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    for (size_t i = self->n; i > 0; i--)
    {
        size_t index = i - 1;
        if (fancy_string_equals(self->array[index], string))
        {
            return index;
        }
    }
    return -1;
}

ssize_t fancy_string_array_last_index_of_value(fancy_string_array_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    for (size_t i = self->n; i > 0; i--)
    {
        size_t index = i - 1;
        if (fancy_string_equals_value(self->array[index], value))
        {
            return index;
        }
    }
    return -1;
}

bool fancy_string_array_includes(fancy_string_array_t const *const self, fancy_string_t const *const string)
{
    assert(self != NULL);
    assert(string != NULL);

    return fancy_string_array_index_of(self, string) != -1;
}

bool fancy_string_array_includes_value(fancy_string_array_t const *const self, char const *const value)
{
    assert(self != NULL);
    assert(value != NULL);

    return fancy_string_array_index_of_value(self, value) != -1;
}

// -----------------------------------------------
//        PRIVATE HELPERS IMPLEMENTATIONS
// -----------------------------------------------

static char *copy_value(char const *const value)
{
    assert(value != NULL);

    char *copied_value;

    size_t n = strlen(value);
    if (n == 0)
    {
        copied_value = my_malloc(sizeof(char));
        copied_value[0] = '\0';
    }
    else
    {
        copied_value = my_malloc(sizeof(char) * (n + 1));
        memcpy(copied_value, value, n);
        copied_value[n] = '\0';
    }

    return copied_value;
}

static bool is_whitespace(char *c)
{
    //  NOTE: Start with `isspace` for now, but check [this](https://en.wikipedia.org/wiki/Whitespace_character)
    //  out if not enough...
    return isspace((int)*c) != 0;
    // switch (*c)
    // {
    // case 0x20: // space
    //     return true;
    // case 0x09: // tab
    //     return true;
    // case 0x0a: // new line
    //     return true;
    // case 0x0d: // ...
    //     return true;
    // default:
    //     return false;
    // }
}

static void to_lower_or_upper_case(fancy_string_t *const self, bool use_lower)
{
    size_t n = fancy_string_size(self);
    if (n == 0)
    {
        return;
    }

    char *new_value = my_malloc(sizeof(char) * (n + 1));
    for (size_t i = 0; i < n; i++)
    {
        if (use_lower)
        {
            new_value[i] = tolower(self->value[i]);
        }
        else
        {
            new_value[i] = toupper(self->value[i]);
        }
    }
    new_value[n] = '\0';
    fancy_string_update_value(self, new_value);
    my_free(new_value);
}

#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED == 1)
// Note about the difference between `_Thread_local` and `__thread`:
// - My understanding is that the former is a C11 specification keyword,
//  while the latter is a compiler-specific specifier (which is implemented by
// both 'gcc' and 'clang')
//  - doc about __thread: https://gcc.gnu.org/onlinedocs/gcc/Thread-Local.html
static _Thread_local fancy_memory_t *memory_tracker_thread_based = NULL;
static fancy_memory_t *memory_tracker_global = NULL;
static pthread_mutex_t memory_tracker_global_mutex = PTHREAD_MUTEX_INITIALIZER;
static _Atomic fancy_string_memory_usage_mode_t memory_tracker_mode = FANCY_STRING_MEMORY_USAGE_MODE_NONE;
#endif

size_t fancy_string_memory_usage_get(void)
{
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED != 1)
    FAIL_WITH_MESSAGE(FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED_REQUIRED_ERROR_MESSAGE);
#else
    if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_NONE)
    {
        return 0;
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL)
    {
        if (memory_tracker_thread_based == NULL)
        {
            return 0;
        }
        return fancy_memory_get_total(memory_tracker_thread_based);
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC)
    {
        if (pthread_mutex_lock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        size_t out = 0;
        if (memory_tracker_global != NULL)
        {
            out = fancy_memory_get_total(memory_tracker_global);
        }
        if (pthread_mutex_unlock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        return out;
    }
    else
    {
        FAIL_WITH_MESSAGE("Unreachable");
    }
#endif
}

void fancy_string_memory_usage_debug(FILE *stream)
{
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED != 1)
    FAIL_WITH_MESSAGE(FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED_REQUIRED_ERROR_MESSAGE);
#else
    if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_NONE)
    {
        fprintf(stream, "Library memory usage tracking feature not enabled.");
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL)
    {
        if (memory_tracker_thread_based == NULL)
        {
            fprintf(stream, "No memory allocated yet.");
        }
        else
        {
            fancy_memory_debug(memory_tracker_thread_based, stream);
        }
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC)
    {
        if (pthread_mutex_lock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        if (memory_tracker_global == NULL)
        {
            fprintf(stream, "No memory allocated yet.");
        }
        else
        {
            fancy_memory_debug(memory_tracker_global, stream);
        }
        if (pthread_mutex_unlock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
    }
    else
    {
        FAIL_WITH_MESSAGE("Unreachable");
    }
#endif
}

void fancy_string_memory_usage_init(fancy_string_memory_usage_mode_t mode)
{
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED != 1)
    FAIL_WITH_MESSAGE(FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED_REQUIRED_ERROR_MESSAGE);
#else
    if (memory_tracker_mode != FANCY_STRING_MEMORY_USAGE_MODE_NONE)
    {
        FAIL_WITH_MESSAGE("Once set to something other than 'FANCY_STRING_MEMORY_USAGE_MODE_NONE', the 'fancy_string_memory_usage_init' method should not be used again.");
    }
    memory_tracker_mode = mode;
#endif
}

fancy_string_memory_usage_mode_t fancy_string_memory_usage_mode(void)
{
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED != 1)
    FAIL_WITH_MESSAGE(FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED_REQUIRED_ERROR_MESSAGE);
#else
    return memory_tracker_mode;
#endif
}

static void *my_malloc(size_t size)
{
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED != 1)
    void *pointer = malloc(size);
    if (pointer == NULL)
    {
        FAIL_WITH_MESSAGE("Call to 'malloc' returned NULL pointer.");
    }
    return pointer;
#else
    if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_NONE)
    {
        void *pointer = malloc(size);
        if (pointer == NULL)
        {
            FAIL_WITH_MESSAGE("Call to 'malloc' returned NULL pointer.");
        }
        return pointer;
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL)
    {
        if (memory_tracker_thread_based == NULL)
        {
            memory_tracker_thread_based = fancy_memory_create();
        }
        void *pointer = fancy_memory_malloc(memory_tracker_thread_based, size);
        if (pointer == NULL)
        {
            FAIL_WITH_MESSAGE("Call to 'malloc' returned NULL pointer.");
        }
        return pointer;
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC)
    {
        if (pthread_mutex_lock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        if (memory_tracker_global == NULL)
        {
            memory_tracker_global = fancy_memory_create();
        }
        void *pointer = fancy_memory_malloc(memory_tracker_global, size);
        if (pointer == NULL)
        {
            FAIL_WITH_MESSAGE("Call to 'malloc' returned NULL pointer.");
        }
        if (pthread_mutex_unlock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        return pointer;
    }
    else
    {
        FAIL_WITH_MESSAGE("Unreachable");
    }
#endif
}

static void my_free(void *ptr)
{
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED != 1)
    free(ptr);
#else
    if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_NONE)
    {
        free(ptr);
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL)
    {
        if (memory_tracker_thread_based == NULL)
        {
            FAIL_WITH_MESSAGE("Memory tracker no set. Something is wrong.");
        }
        fancy_memory_free(memory_tracker_thread_based, ptr);
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC)
    {
        if (pthread_mutex_lock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        if (memory_tracker_global == NULL)
        {
            FAIL_WITH_MESSAGE("Memory tracker no set. Something is wrong.");
        }
        fancy_memory_free(memory_tracker_global, ptr);
        if (pthread_mutex_unlock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
    }
    else
    {
        FAIL_WITH_MESSAGE("Unreachable");
    }
#endif
}

static void *my_realloc(void *ptr, size_t size)
{
#if (FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED != 1)
    void *pointer = realloc(ptr, size);
    if (pointer == NULL)
    {
        FAIL_WITH_MESSAGE("Call to 'realloc' returned NULL pointer.");
    }
    return pointer;
#else
    if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_NONE)
    {
        void *pointer = realloc(ptr, size);
        if (pointer == NULL)
        {
            FAIL_WITH_MESSAGE("Call to 'realloc' returned NULL pointer.");
        }
        return pointer;
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL)
    {
        if (memory_tracker_thread_based == NULL)
        {
            FAIL_WITH_MESSAGE("Memory tracker no set. Something is wrong.");
        }
        void *pointer = fancy_memory_realloc(memory_tracker_thread_based, ptr, size);
        if (pointer == NULL)
        {
            FAIL_WITH_MESSAGE("Call to 'realloc' returned NULL pointer.");
        }
        return pointer;
    }
    else if (memory_tracker_mode == FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC)
    {
        if (pthread_mutex_lock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        if (memory_tracker_global == NULL)
        {
            FAIL_WITH_MESSAGE("Memory tracker no set. Something is wrong.");
        }
        void *pointer = fancy_memory_realloc(memory_tracker_global, ptr, size);
        if (pointer == NULL)
        {
            FAIL_WITH_MESSAGE("Call to 'realloc' returned NULL pointer.");
        }
        if (pthread_mutex_unlock(&memory_tracker_global_mutex) != 0)
        {
            FAIL_WITH_MESSAGE("Failed to acquire global memory tracker mutex.");
        }
        return pointer;
    }
    else
    {
        FAIL_WITH_MESSAGE("Unreachable");
    }
#endif
}
