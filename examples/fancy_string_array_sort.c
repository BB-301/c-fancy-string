#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "fancy_string.h"

#define IGNORE_UNUSED(x) ((void)(x))

static bool callback(fancy_string_t const *const string_1, fancy_string_t const *const string_2, void *context)
{
    IGNORE_UNUSED(context);

    // NOTE: This callback implementation is essentially the same as the one used
    // by the `fancy_string_array_sort_values` and `fancy_string_array_sorted_values`
    // methods.

    size_t n_1 = fancy_string_size(string_1);
    size_t n_2 = fancy_string_size(string_2);

    char *value_1 = fancy_string_value(string_1);
    char *value_2 = fancy_string_value(string_2);

    size_t n = n_1 > n_2 ? n_2 : n_1;

    for (size_t i = 0; i < n; i++)
    {
        if (value_1[i] == value_2[i])
        {
            continue;
        }
        bool out = value_1[i] < value_2[i];
        free(value_1);
        free(value_2);
        return out;
    }

    free(value_1);
    free(value_2);

    if (n_1 == n_2)
    {
        return true;
    }
    else if (n_2 < n_1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int main(void)
{
    char *value = "John Alberto Eric Nicholas Isaac Henry Paul Ronald Keith Bob Rob Anna";
    char *sorted = "Alberto Anna Bob Eric Henry Isaac John Keith Nicholas Paul Rob Ronald";

    fancy_string_t *s = fancy_string_create(value);
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    fancy_string_array_print(a, stdout, true);

    fancy_string_array_sort(a, callback, NULL);

    fancy_string_t *s_sorted = fancy_string_array_join_by_value(a, " ");
    assert(fancy_string_equals_value(s_sorted, sorted));

    fancy_string_array_print(a, stdout, true);

    fancy_string_destroy(s_sorted);
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);

    return 0;
}
