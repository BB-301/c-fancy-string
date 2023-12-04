#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

#define IGNORE_UNUSED(x) ((void)(x))

static void updater_callback(fancy_string_t *const match, size_t start, size_t end, fancy_string_t const *const string, void *context);

struct updater_callback_context
{
    char *uppercase_if_equals_to;
};

int main(void)
{
    fancy_string_t *s = fancy_string_create("My name is Francis and I wrote this library for fun!");
    fancy_string_t *pattern = fancy_string_create("[A-Z][a-z]*");

    fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
    assert(re != NULL);

    fancy_string_regex_debug(re, stdout, true);

    struct updater_callback_context context = {.uppercase_if_equals_to = "Francis"};
    fancy_string_t *updated_string = fancy_string_regex_to_string_with_updated_matches(re, updater_callback, &context);
    assert(fancy_string_equals_value(updated_string, "[[My]] name is [[FRANCIS]] and [[I]] wrote this library for fun!"));

    fancy_string_print(updated_string, stdout, true);

    fancy_string_destroy(updated_string);
    fancy_string_regex_destroy(re);
    fancy_string_destroy(pattern);
    fancy_string_destroy(s);

    return 0;
}

static void updater_callback(fancy_string_t *const match, size_t start, size_t end, fancy_string_t const *const string, void *context)
{
    IGNORE_UNUSED(start);
    IGNORE_UNUSED(end);
    IGNORE_UNUSED(string);

    struct updater_callback_context *c = (struct updater_callback_context *)context;

    if (fancy_string_equals_value(match, c->uppercase_if_equals_to))
    {
        fancy_string_uppercase(match);
    }

    fancy_string_prepend_value(match, "[[");
    fancy_string_append_value(match, "]]");
}
