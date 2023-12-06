#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_t *s = fancy_string_create("this is a test");
    // end = 6 to get 0..5 range
    fancy_string_t *s_sub = fancy_string_substring(s, 0, 6);
    assert(fancy_string_equals_value(s_sub, "this i"));
    fancy_string_destroy(s_sub);
    fancy_string_destroy(s);
    return 0;
}
