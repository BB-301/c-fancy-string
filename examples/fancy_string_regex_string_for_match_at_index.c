#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_t *s = fancy_string_create("Hello, lib<fancy_string>! My name is Francis. Can I call you lib<fancy-string> instead?");
    fancy_string_t *pattern = fancy_string_create("[a-z]*<[-_a-z]*>");
    fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
    assert(re != NULL);
    assert(fancy_string_regex_match_count(re) == 2);

    {
        fancy_string_t *lib_name = fancy_string_regex_string_for_match_at_index(re, 0);
        assert(re != NULL);
        assert(fancy_string_equals_value(lib_name, "lib<fancy_string>"));
        fancy_string_print(lib_name, stdout, true);
        fancy_string_destroy(lib_name);
    }

    {
        fancy_string_t *lib_name = fancy_string_regex_string_for_match_at_index(re, 1);
        assert(re != NULL);
        assert(fancy_string_equals_value(lib_name, "lib<fancy-string>"));
        fancy_string_print(lib_name, stdout, true);
        fancy_string_destroy(lib_name);
    }

    fancy_string_regex_destroy(re);
    fancy_string_destroy(pattern);
    fancy_string_destroy(s);

    return 0;
}
