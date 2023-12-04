#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

#define IGNORE_UNUSED(x) ((void)(x))

struct my_context
{
    char *starts_with;
};

static bool callback(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(index);
    IGNORE_UNUSED(array);

    struct my_context *c = (struct my_context *)context;

    return fancy_string_starts_with_value(string, c->starts_with);
}

int main(void)
{
    char *value = "Julie John Jeff Karina Francis Joe Lucas Eric";
    char *filtered = "Julie John Jeff Joe";

    struct my_context context = {.starts_with = "J"};

    fancy_string_t *s = fancy_string_create(value);
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    fancy_string_array_print(a, stdout, true);

    fancy_string_array_filter(a, callback, &context);

    fancy_string_t *s_filtered = fancy_string_array_join_by_value(a, " ");
    assert(fancy_string_equals_value(s_filtered, filtered));

    fancy_string_array_print(a, stdout, true);

    fancy_string_destroy(s_filtered);
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);

    return 0;
}
