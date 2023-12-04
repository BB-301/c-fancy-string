#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    // Instantiate a new string object with initial internal state "Hello, World!"
    fancy_string_t *s = fancy_string_create("Hello, World!");

    // Print a summary of the object to `stdout`
    fancy_string_print(s, stdout, true);
    // Prints:
    // fancy_string_t[13](Hello, World!)

    // Replace the first (and only) occurrence of 'World' with 'lib<fancy_string>'.
    fancy_string_replace_value(s, "World", "lib<fancy_string>", 1);
    //  Make sure that the string has been updated like we wanted to...
    assert(fancy_string_equals_value(s, "Hello, lib<fancy_string>!"));
    // ... and print a summary of updated object to `stdout`.
    fancy_string_print(s, stdout, true);
    // Prints:
    // fancy_string_t[25](Hello, lib<fancy_string>!)

    // Prepend the string object's internal state with 'Greetings: '.
    fancy_string_prepend_value(s, "Greetings: ");
    // Make sure that the string has been updated like we wanted to...
    assert(fancy_string_equals_value(s, "Greetings: Hello, lib<fancy_string>!"));
    // ... and print a summary of updated object to `stdout`.
    fancy_string_print(s, stdout, true);
    // Prints:
    // fancy_string_t[36](Greetings: Hello, lib<fancy_string>!)

    // So far, we've used methods suffixed with `_value`, which means that
    // the take as argument a string literal. But the library also have the
    // matching methods for string objects. Let's append something to our string:
    {
        // Instantiate a new string object containing the suffix to be appended to `s`.
        fancy_string_t *suffix = fancy_string_create(" My name is Francis!");
        // Append `suffix` to `s`.
        fancy_string_append(s, suffix);
        // Destroy the `suffix` object, since it's no longer needed.
        fancy_string_destroy(suffix);

        // Now, let's confirm that our string object has been updated as expected:
        assert(fancy_string_equals_value(s, "Greetings: Hello, lib<fancy_string>! My name is Francis!"));
        // and let's print a summary of updated object to `stdout`.
        fancy_string_print(s, stdout, true);
        // Prints:
        // fancy_string_t[56](Greetings: Hello, lib<fancy_string>! My name is Francis!)
    }

    // We could go on forever, because there are a lot of methods in this library's
    // API. But let's see two more.

    // Regular expression with lib<fancy_string>
    {
        // Instantiate a string object containing the pattern for which to match.
        // In this case, we are trying to match "lib<fancy_string>"
        fancy_string_t *pattern = fancy_string_create("[a-z]*<[_a-z]*>");
        // Instantiate the regular expression object.
        fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
        // Confirm that no error occurred.
        assert(re != NULL);
        // Print a verbose summary of the regular expression object to `stdout`.
        fancy_string_regex_debug(re, stdout, true);
        // Confirm that we have a match.
        assert(fancy_string_regex_match_count(re) == 1);
        // Retrieve the "match" info at `index = 0`.
        fancy_string_regex_match_info_t match_info = fancy_string_regex_match_info_for_index(re, 0);
        // Confirm that we have something at `index = 0`.
        assert(match_info.index == 0);
        // Create a substring of `s` for the matched range (i.e., from `start` to `end`)
        fancy_string_t *match = fancy_string_substring(s, match_info.start, match_info.end);
        // Confirm that the match corresponds to 'lib<fancy_string>'.
        assert(fancy_string_equals_value(match, "lib<fancy_string>"));
        // Destroy the string object containing the substring.
        fancy_string_destroy(match);
        // Destroy the regular expression object.
        fancy_string_regex_destroy(re);
        // Destroy the string object that stores the pattern.
        fancy_string_destroy(pattern);
    }

    //  The library also has (string) array methods.
    {
        // Instantiate a string object with single-space-separated names.
        fancy_string_t *names = fancy_string_create("john eric arold joe henry zoe joey ralf");
        // Split the string at ' ', an unlimited number of times (n_max_splits = -1 does that).
        fancy_string_array_t *a = fancy_string_split_by_value(names, " ", -1);
        // Print a summary of the array to `stdout`.
        fancy_string_array_print(a, stdout, true);
        // Sort the array (in ascending order)
        fancy_string_array_sort_values(a);
        // Join the array back into a string.
        fancy_string_t *sorted_names = fancy_string_array_join_by_value(a, " ");
        // Confirm that the names have been sorted.
        assert(fancy_string_equals_value(sorted_names, "arold eric henry joe joey john ralf zoe"));
        // Print the sorted string to `stdout`
        fancy_string_print(sorted_names, stdout, true);
        // Destroy the three objects.
        fancy_string_destroy(sorted_names);
        fancy_string_array_destroy(a);
        fancy_string_destroy(names);
    }

    // The library contains many more methods, so please have a look at the
    // API documentation and the `examples` directory for more!

    // Destroy the string object.
    fancy_string_destroy(s);

    // Exit program with success status.
    exit(EXIT_SUCCESS);
}
