#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

#define IGNORE_UNUSED(x) ((void)(x))

struct my_context
{
    char *old_value;
    char *new_value;
};

static fancy_string_t *callback(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(index);
    IGNORE_UNUSED(array);

    struct my_context *c = (struct my_context *)context;

    fancy_string_t *clone = fancy_string_clone(string);

    fancy_string_replace_value(clone, c->old_value, c->new_value, -1);

    return clone;
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

    struct my_context context = {.old_value = "a", .new_value = "A"};
    fancy_string_array_t *a_mapped = fancy_string_array_mapped(a, callback, &context);

    fancy_string_array_print(a, stdout, true);

    fancy_string_t *s = fancy_string_array_join_by_value(a_mapped, " ");
    assert(fancy_string_equals_value(s, "AnnA nAthAn john jules liz"));

    fancy_string_array_destroy(a_mapped);
    fancy_string_destroy(s);
    fancy_string_array_destroy(a);

    return 0;
}
