#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    char *value = "John Alberto Eric Nicholas Isaac Henry Paul Ronald Keith Bob Rob Anna";
    char *sorted = "Alberto Anna Bob Eric Henry Isaac John Keith Nicholas Paul Rob Ronald";

    fancy_string_t *s = fancy_string_create(value);
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    fancy_string_array_print(a, stdout, true);

    fancy_string_array_sort_values(a);

    fancy_string_t *s_sorted = fancy_string_array_join_by_value(a, " ");
    assert(fancy_string_equals_value(s_sorted, sorted));

    fancy_string_array_print(a, stdout, true);

    fancy_string_destroy(s_sorted);
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);

    return 0;
}
