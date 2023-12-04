#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

#define IGNORE_UNUSED(x) ((void)(x))

struct my_context
{
    char *prefix;
    size_t skip_indexes_above;
};

static void callback(fancy_string_t *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(array);

    struct my_context *c = (struct my_context *)context;

    if (index > c->skip_indexes_above)
    {
        return;
    }

    fancy_string_prepend_value(string, c->prefix);
}

int main(void)
{
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "anna");
    fancy_string_array_push_value(a, "nathan");
    fancy_string_array_push_value(a, "john");
    fancy_string_array_push_value(a, "jules");
    fancy_string_array_push_value(a, "liz");

    fancy_string_array_print(a, stdout, true);

    struct my_context context = {.prefix = "HEY_", .skip_indexes_above = 2};
    fancy_string_array_for_each(a, callback, &context);

    fancy_string_array_print(a, stdout, true);

    fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
    assert(fancy_string_equals_value(s, "HEY_anna HEY_nathan HEY_john jules liz"));

    fancy_string_destroy(s);
    fancy_string_array_destroy(a);

    return 0;
}
