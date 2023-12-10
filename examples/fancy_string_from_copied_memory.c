#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    char *message = "I only want this part; not this one.";

    {
        fancy_string_t *s = fancy_string_from_copied_memory(message, 21);
        assert(fancy_string_equals_value(s, "I only want this part"));
        fancy_string_destroy(s);
    }

    {
        fancy_string_t *s = fancy_string_from_copied_memory(message + 7, 4);
        assert(fancy_string_equals_value(s, "want"));
        fancy_string_destroy(s);
    }

    {
        char tmp[5] = {'h', 'e', 'l', 'l', 'o'};
        {
            fancy_string_t *s = fancy_string_from_copied_memory(tmp, 5);
            assert(fancy_string_equals_value(s, "hello"));
            fancy_string_destroy(s);
        }
    }
    {
        char tmp[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
        {
            // NOTE: Beyond tmp[5], The size `n` does not matter, because we have a null character.
            fancy_string_t *s = fancy_string_from_copied_memory(tmp, 10);
            assert(fancy_string_equals_value(s, "hello"));
            fancy_string_destroy(s);
        }
    }

    return 0;
}
