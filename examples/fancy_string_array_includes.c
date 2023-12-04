#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "joey");
    fancy_string_array_push_value(a, "francis");
    fancy_string_array_push_value(a, "eric");

    assert(fancy_string_array_includes_value(a, "joey"));
    assert(!fancy_string_array_includes_value(a, "caesar"));

    {
        fancy_string_t *name = fancy_string_create("joey");

        assert(fancy_string_array_includes(a, name));

        fancy_string_update_value(name, "caesar");
        assert(!fancy_string_array_includes(a, name));

        fancy_string_destroy(name);
    }

    fancy_string_array_destroy(a);

    return 0;
}
