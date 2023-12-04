#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_t *s = fancy_string_create("lib<fancy_string> is a C library for easy and fun string manipulation.");
    fancy_string_t *pattern = fancy_string_create("fancy[_a-z]*|string|lib[a-z]*");

    fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
    assert(re != NULL);

    fancy_string_regex_debug(re, stdout, true);
    assert(fancy_string_regex_match_count(re) == 4);

    fancy_string_array_t *matched_words = fancy_string_regex_matches_to_strings(re);

    assert(fancy_string_array_size(matched_words) == 4);
    assert(fancy_string_array_includes_value(matched_words, "library"));
    assert(fancy_string_array_includes_value(matched_words, "fancy_string"));

    fancy_string_array_destroy(matched_words);
    fancy_string_regex_destroy(re);
    fancy_string_destroy(pattern);
    fancy_string_destroy(s);

    return 0;
}
