#include <assert.h>

#include "fancy_string.h"

#define IGNORE_UNUSED(x) ((void)(x))

enum my_context_type
{
    MY_CONTEXT_TYPE_STARTS_WITH,
    MY_CONTEXT_TYPE_ENDS_WITH
};

struct my_context
{
    char *string;
    enum my_context_type type;
};

static bool my_callback(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(index);
    IGNORE_UNUSED(array);

    struct my_context *c = (struct my_context *)context;

    switch (c->type)
    {
    case MY_CONTEXT_TYPE_STARTS_WITH:
        return fancy_string_starts_with_value(string, c->string);
    case MY_CONTEXT_TYPE_ENDS_WITH:
        return fancy_string_ends_with_value(string, c->string);
    }
}

int main(void)
{
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "Alberto");
    fancy_string_array_push_value(a, "Ricardo");
    fancy_string_array_push_value(a, "Angelo");
    fancy_string_array_push_value(a, "John");
    fancy_string_array_push_value(a, "Joe");
    fancy_string_array_push_value(a, "Eric");
    fancy_string_array_push_value(a, "Francis");
    fancy_string_array_push_value(a, "Jules");

    struct my_context context;

    context = (struct my_context){.type = MY_CONTEXT_TYPE_STARTS_WITH, .string = "J"};
    assert(fancy_string_array_find_index(a, my_callback, &context) == 3);

    context = (struct my_context){.type = MY_CONTEXT_TYPE_STARTS_WITH, .string = "A"};
    assert(fancy_string_array_find_index(a, my_callback, &context) == 0);

    context = (struct my_context){.type = MY_CONTEXT_TYPE_STARTS_WITH, .string = "Z"};
    assert(fancy_string_array_find_index(a, my_callback, &context) == -1);

    context = (struct my_context){.type = MY_CONTEXT_TYPE_ENDS_WITH, .string = "o"};
    assert(fancy_string_array_find_index(a, my_callback, &context) == 0);

    context = (struct my_context){.type = MY_CONTEXT_TYPE_ENDS_WITH, .string = "s"};
    assert(fancy_string_array_find_index(a, my_callback, &context) == 6);

    fancy_string_array_destroy(a);

    return 0;
}
