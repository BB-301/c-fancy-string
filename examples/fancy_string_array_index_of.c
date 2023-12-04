#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_t *s = fancy_string_create("joe john jimmy joey jeffry joey jeff julie joe jeff julia julian jimmy");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);

    // First index ("joey")
    assert(fancy_string_array_index_of_value(a, "joey") == 3);
    // Last index ("joey")
    assert(fancy_string_array_last_index_of_value(a, "joey") == 5);

    // First index ("julian")
    assert(fancy_string_array_index_of_value(a, "julian") == 11);
    // Last index ("julian")
    assert(fancy_string_array_last_index_of_value(a, "julian") == 11);

    // No match
    assert(fancy_string_array_last_index_of_value(a, "caesar") == -1);

    // Using a string object instead of a value...
    {
        fancy_string_t *name = fancy_string_create("joe");

        // First index ("joe")
        assert(fancy_string_array_index_of(a, name) == 0);
        // Last index ("joe")
        assert(fancy_string_array_last_index_of(a, name) == 8);

        fancy_string_destroy(name);
    }

    fancy_string_array_destroy(a);
    fancy_string_destroy(s);

    return 0;
}
