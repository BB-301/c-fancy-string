#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_array_t *a = fancy_string_array_create_with_values(
        "one",
        "two",
        "three",
        "four",
        "five",
        NULL);

    assert(fancy_string_array_size(a) == 5);
    fancy_string_array_print(a, stdout, true);

    fancy_string_t *s = fancy_string_array_join_by_value(a, " ");

    assert(fancy_string_equals_value(s, "one two three four five"));
    fancy_string_print(s, stdout, true);

    fancy_string_destroy(s);
    fancy_string_array_destroy(a);

    return 0;
}
