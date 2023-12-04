#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    fancy_string_array_push_value(a, "five");
    fancy_string_array_push_value(a, "six");
    fancy_string_array_push_value(a, "seven");
    fancy_string_array_push_value(a, "eight");
    fancy_string_array_push_value(a, "nine");

    fancy_string_array_print(a, stdout, true);

    // Here we use the `splice` method, with the arguments enumerated below, to remove all the elements
    // starting at position 3:
    // self = a; index = 3; delete_count = -1; optional_new_strings = NULL
    fancy_string_array_t *removed = fancy_string_array_splice(a, 3, -1, NULL);

    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "one two three"));
        fancy_string_destroy(s);
    }
    fancy_string_array_print(a, stdout, true);

    {
        fancy_string_t *s = fancy_string_array_join_by_value(removed, " ");
        assert(fancy_string_equals_value(s, "four five six seven eight nine"));
        fancy_string_destroy(s);
    }
    fancy_string_array_print(removed, stdout, true);

    //  Now, we are going to insert back the removed elements (i.e., copies of those elements)
    //  using the `splice` method with the following arguments:
    //  self = a; index = 3; delete_count = 0; optional_new_strings = removed
    fancy_string_array_t *removed_2 = fancy_string_array_splice(a, 3, 0, removed);

    assert(fancy_string_array_is_empty(removed_2));

    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "one two three four five six seven eight nine"));
        fancy_string_destroy(s);
    }
    fancy_string_array_print(a, stdout, true);

    fancy_string_array_t *new_elements = fancy_string_array_create();
    fancy_string_array_push_value(new_elements, "twelve");
    fancy_string_array_push_value(new_elements, "thirteen");

    // Finally, we use the `splice` method to insert `new_elements` starting at
    // position 11, which is out of bounds, so we expect to have two empty elements
    // at position 9 and 10, followed by `twelve` and `thirteen` at positions 11
    // and 12, respectively. Our arguments are:
    // self = a; index = 11; delete_count = 0; optional_new_strings = new_elements
    fancy_string_array_t *removed_3 = fancy_string_array_splice(a, 11, 0, new_elements);

    assert(fancy_string_array_size(a) == 13);
    assert(fancy_string_array_is_empty(removed_3));

    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, "_");
        assert(fancy_string_equals_value(s, "one_two_three_four_five_six_seven_eight_nine___twelve_thirteen"));
        fancy_string_destroy(s);
    }
    fancy_string_array_print(a, stdout, true);

    fancy_string_array_destroy(removed_3);
    fancy_string_array_destroy(new_elements);
    fancy_string_array_destroy(removed_2);
    fancy_string_array_destroy(removed);
    fancy_string_array_destroy(a);

    return 0;
}
