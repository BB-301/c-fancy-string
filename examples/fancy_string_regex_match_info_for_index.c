#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_t *s = fancy_string_create("jules julie julius jean jeffrey joey john joe");
    // We match character sequences that start with "j" and end with "e " or "e" at the end of a line,
    //  with anything in the middle corresponding to ACSII lowercase letters (i.e. a to z). In this case, that
    // means that "julie " and "joe" will be matched.
    fancy_string_t *p = fancy_string_create("j[a-z]*e($|[ ]{1})");

    fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);

    fancy_string_regex_debug(re, stdout, true);

    assert(fancy_string_regex_match_count(re) == 2);

    {
        fancy_string_regex_match_info_t info = fancy_string_regex_match_info_for_index(re, 0);
        assert(info.index == 0);
        fancy_string_t *name = fancy_string_substring(s, info.start, info.end);
        fancy_string_trim(name);
        assert(fancy_string_equals_value(name, "julie"));
        fancy_string_destroy(name);
    }

    {
        fancy_string_regex_match_info_t info = fancy_string_regex_match_info_for_index(re, 1);
        assert(info.index == 1);
        fancy_string_t *name = fancy_string_substring(s, info.start, info.end);
        assert(fancy_string_equals_value(name, "joe"));
        fancy_string_destroy(name);
    }

    {
        fancy_string_regex_match_info_t info = fancy_string_regex_match_info_for_index(re, 2);
        assert(info.index == -1);
    }

    fancy_string_regex_destroy(re);
    fancy_string_destroy(p);
    fancy_string_destroy(s);

    return 0;
}
