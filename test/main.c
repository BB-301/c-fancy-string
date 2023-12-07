/*
    Copyright (c) 2023 BB-301 <fw3dg3@gmail.com>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the “Software”), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "fancy_string.h"

#define LOG_FUNCTION_CALLS

#define TEST_LOOP_NUMBER_OF_ITERATIONS (100000)
#define TEST_LOOP_SLEEP_DURATION_SECS (1)
#ifndef FANCY_STRING_TEST_LOOP_ENABLED
#define FANCY_STRING_TEST_LOOP_ENABLED (0)
#endif

#define PRINT_TO_STDOUT_DISABLED (FANCY_STRING_TEST_LOOP_ENABLED)

#define IGNORE_UNUSED(x) ((void)(x))

#ifdef LOG_FUNCTION_CALLS
#define LOG()                      \
    if (!PRINT_TO_STDOUT_DISABLED) \
    fprintf(stdout, "Running \033[32m%s()\033[0m ...\n", __func__)
#else
#define LOG() (void)0
#endif

void test_fancy_string_regex_create(void);
void test_fancy_string_regex_to_string_with_updated_matches(void);
void test_fancy_string_regex_destroy(void);
void test_fancy_string_regex_debug(void);
void test_fancy_string_regex_matches_to_strings(void);
void test_fancy_string_regex_replaced_matches(void);
void test_fancy_string_regex_split_at_matches(void);
void test_fancy_string_regex_match_count(void);
void test_fancy_string_regex_has_match(void);
void test_fancy_string_regex_max_number_of_matches(void);
void test_fancy_string_regex_max_number_of_matches_reached(void);
void test_fancy_string_regex_string(void);
void test_fancy_string_regex_pattern(void);
void test_fancy_string_regex_match_info_for_index(void);
void test_fancy_string_array_includes_value(void);
void test_fancy_string_array_includes(void);
void test_fancy_string_array_last_index_of_value(void);
void test_fancy_string_array_last_index_of(void);
void test_fancy_string_array_index_of_value(void);
void test_fancy_string_array_index_of(void);
void test_fancy_string_array_every(void);
void test_fancy_string_array_none(void);
void test_fancy_string_array_some(void);
void test_fancy_string_array_find_last(void);
void test_fancy_string_array_find_last_index(void);
void test_fancy_string_array_find(void);
void test_fancy_string_array_find_index(void);
void test_fancy_string_array_spliced(void);
void test_fancy_string_array_splice_and_destroy(void);
void test_fancy_string_array_splice(void);
void test_fancy_string_array_slice_and_destroy(void);
void test_fancy_string_array_sliced(void);
void test_fancy_string_array_filtered(void);
void test_fancy_string_array_filter(void);
void test_fancy_string_array_sorted(void);
void test_fancy_string_array_sort(void);
void test_fancy_string_array_sorted_values(void);
void test_fancy_string_array_sort_values(void);
void test_fancy_string_array_mapped(void);
void test_fancy_string_array_for_each(void);
void test_fancy_string_array_reversed(void);
void test_fancy_string_array_reverse(void);
void test_fancy_string_array_join(void);
void test_fancy_string_array_join_by_value(void);
void test_fancy_string_array_last_value(void);
void test_fancy_string_array_last(void);
void test_fancy_string_array_first_value(void);
void test_fancy_string_array_first(void);
void test_fancy_string_array_insert(void);
void test_fancy_string_array_insert_value(void);
void test_fancy_string_array_shift_and_destroy(void);
void test_fancy_string_array_shift(void);
void test_fancy_string_array_pop_and_destroy(void);
void test_fancy_string_array_pop(void);
void test_fancy_string_array_remove_and_destroy(void);
void test_fancy_string_array_remove(void);
void test_fancy_string_array_get_value(void);
void test_fancy_string_array_get(void);
void test_fancy_string_array_push(void);
void test_fancy_string_array_push_value(void);
void test_fancy_string_array_push_values(void);
void test_fancy_string_array_is_empty(void);
void test_fancy_string_array_size(void);
void test_fancy_string_array_clear(void);
void test_fancy_string_array_print(void);
void test_fancy_string_array_destroy(void);
void test_fancy_string_array_append_and_destroy(void);
void test_fancy_string_array_append(void);
void test_fancy_string_array_clone(void);
void test_fancy_string_array_create_with_values(void);
void test_fancy_string_array_create(void);
void test_fancy_string_uppercased(void);
void test_fancy_string_uppercase(void);
void test_fancy_string_lowercased(void);
void test_fancy_string_lowercase(void);
void test_fancy_string_replaced(void);
void test_fancy_string_replaced_value(void);
void test_fancy_string_replace(void);
void test_fancy_string_replace_value(void);
void test_fancy_string_padded_end(void);
void test_fancy_string_pad_end(void);
void test_fancy_string_padded_start(void);
void test_fancy_string_pad_start(void);
void test_fancy_string_split(void);
void test_fancy_string_split_by_value(void);
void test_fancy_string_trimmed(void);
void test_fancy_string_trim(void);
void test_fancy_string_trimmed_right(void);
void test_fancy_string_trim_right(void);
void test_fancy_string_trimmed_left(void);
void test_fancy_string_trim_left(void);
void test_fancy_string_substring(void);
void test_fancy_string_contains(void);
void test_fancy_string_contains_value(void);
void test_fancy_string_index_of(void);
void test_fancy_string_index_of_value(void);
void test_fancy_string_ends_with(void);
void test_fancy_string_ends_with_value(void);
void test_fancy_string_starts_with(void);
void test_fancy_string_starts_with_value(void);
void test_fancy_string_equals(void);
void test_fancy_string_equals_value(void);
void test_fancy_string_prepend(void);
void test_fancy_string_prepend_value(void);
void test_fancy_string_append(void);
void test_fancy_string_append_value(void);
void test_fancy_string_value(void);
void test_fancy_string_update(void);
void test_fancy_string_update_value(void);
void test_fancy_string_clear(void);
void test_fancy_string_is_empty(void);
void test_fancy_string_size(void);
void test_fancy_string_print(void);
void test_fancy_string_destroy(void);
void test_fancy_string_clone(void);
void test_fancy_string_create_empty(void);
void test_fancy_string_create_repeat(void);
void test_fancy_string_from_stream(void);
void test_fancy_string_from_stream_next_line(void);
void test_fancy_string_create(void);
void test_fancy_string_library_version_print(void);
void test_fancy_string_library_version(void);

void run_tests(void)
{
    test_fancy_string_library_version();
    test_fancy_string_library_version_print();
    test_fancy_string_create();
    test_fancy_string_from_stream();
    test_fancy_string_from_stream_next_line();
    test_fancy_string_create_repeat();
    test_fancy_string_create_empty();
    test_fancy_string_clone();
    test_fancy_string_destroy();
    test_fancy_string_print();
    test_fancy_string_size();
    test_fancy_string_is_empty();
    test_fancy_string_clear();
    test_fancy_string_update_value();
    test_fancy_string_update();
    test_fancy_string_value();
    test_fancy_string_append_value();
    test_fancy_string_append();
    test_fancy_string_prepend_value();
    test_fancy_string_prepend();
    test_fancy_string_equals_value();
    test_fancy_string_equals();
    test_fancy_string_starts_with_value();
    test_fancy_string_starts_with();
    test_fancy_string_ends_with_value();
    test_fancy_string_ends_with();
    test_fancy_string_index_of_value();
    test_fancy_string_index_of();
    test_fancy_string_contains_value();
    test_fancy_string_contains();
    test_fancy_string_substring();
    test_fancy_string_trim_left();
    test_fancy_string_trimmed_left();
    test_fancy_string_trim_right();
    test_fancy_string_trimmed_right();
    test_fancy_string_trim();
    test_fancy_string_trimmed();
    test_fancy_string_split_by_value();
    test_fancy_string_split();
    test_fancy_string_pad_start();
    test_fancy_string_padded_start();
    test_fancy_string_pad_end();
    test_fancy_string_padded_end();
    test_fancy_string_replace_value();
    test_fancy_string_replace();
    test_fancy_string_replaced_value();
    test_fancy_string_replaced();
    test_fancy_string_lowercase();
    test_fancy_string_lowercased();
    test_fancy_string_uppercase();
    test_fancy_string_uppercased();
    test_fancy_string_array_create_with_values();
    test_fancy_string_array_create();
    test_fancy_string_array_clone();
    test_fancy_string_array_append();
    test_fancy_string_array_append_and_destroy();
    test_fancy_string_array_destroy();
    test_fancy_string_array_print();
    test_fancy_string_array_clear();
    test_fancy_string_array_size();
    test_fancy_string_array_is_empty();
    test_fancy_string_array_push_value();
    test_fancy_string_array_push_values();
    test_fancy_string_array_push();
    test_fancy_string_array_get();
    test_fancy_string_array_get_value();
    test_fancy_string_array_remove();
    test_fancy_string_array_remove_and_destroy();
    test_fancy_string_array_pop();
    test_fancy_string_array_pop_and_destroy();
    test_fancy_string_array_shift();
    test_fancy_string_array_shift_and_destroy();
    test_fancy_string_array_insert_value();
    test_fancy_string_array_insert();
    test_fancy_string_array_first();
    test_fancy_string_array_first_value();
    test_fancy_string_array_last();
    test_fancy_string_array_join_by_value();
    test_fancy_string_array_join();
    test_fancy_string_array_reverse();
    test_fancy_string_array_reversed();
    test_fancy_string_array_for_each();
    test_fancy_string_array_mapped();
    test_fancy_string_array_sort();
    test_fancy_string_array_sorted();
    test_fancy_string_array_sort_values();
    test_fancy_string_array_sorted_values();
    test_fancy_string_array_filter();
    test_fancy_string_array_filtered();
    test_fancy_string_array_sliced();
    test_fancy_string_array_slice_and_destroy();
    test_fancy_string_array_splice();
    test_fancy_string_array_splice_and_destroy();
    test_fancy_string_array_spliced();
    test_fancy_string_array_find_index();
    test_fancy_string_array_find();
    test_fancy_string_array_find_last_index();
    test_fancy_string_array_find_last();
    test_fancy_string_array_some();
    test_fancy_string_array_none();
    test_fancy_string_array_every();
    test_fancy_string_array_index_of();
    test_fancy_string_array_index_of_value();
    test_fancy_string_array_last_index_of();
    test_fancy_string_array_last_index_of_value();
    test_fancy_string_array_includes();
    test_fancy_string_array_includes_value();
    test_fancy_string_regex_match_info_for_index();
    test_fancy_string_regex_pattern();
    test_fancy_string_regex_string();
    test_fancy_string_regex_max_number_of_matches_reached();
    test_fancy_string_regex_max_number_of_matches();
    test_fancy_string_regex_has_match();
    test_fancy_string_regex_match_count();
    test_fancy_string_regex_split_at_matches();
    test_fancy_string_regex_replaced_matches();
    test_fancy_string_regex_matches_to_strings();
    test_fancy_string_regex_debug();
    test_fancy_string_regex_destroy();
    test_fancy_string_regex_to_string_with_updated_matches();
    test_fancy_string_regex_create();
}

int main(void)
{
    // NOTE: This call will crash the process if the library has been built
    // using `FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED = 0`. By default, the
    // library gets built with `FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED = 1` if
    // not specified.
    fancy_string_memory_usage_init(FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL);

    if (FANCY_STRING_TEST_LOOP_ENABLED)
    {
        for (size_t i = 0; i < TEST_LOOP_NUMBER_OF_ITERATIONS; i++)
        {
            if (i % 100 == 0)
            {
                printf("[%zu] Test loop\n", i);
            }
            if ((i > 0) && (i % 1000 == 0))
            {
                printf("\tMemory in use: %zu bytes\n", fancy_string_memory_usage_get());
                printf("\tSleeping %d second(s)...\n", TEST_LOOP_SLEEP_DURATION_SECS);
                sleep(TEST_LOOP_SLEEP_DURATION_SECS);
            }
            run_tests();
        }
    }
    else
    {
        run_tests();
        printf("\tMemory in use after testing: %zu bytes\n", fancy_string_memory_usage_get());
    }

    return 0;
}

void test_fancy_string_regex_create(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("lib<fancy_string> is a C library for easy and fun string manipulation.");
        fancy_string_t *pattern = fancy_string_create("fancy[_a-z]*|string|lib[a-z]*");
        fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
        assert(re != NULL);
        assert(fancy_string_regex_match_count(re) == 4);
        fancy_string_array_t *matched_words = fancy_string_regex_matches_to_strings(re);
        assert(fancy_string_array_size(matched_words) == 4);
        assert(fancy_string_array_includes_value(matched_words, "library"));
        assert(fancy_string_array_includes_value(matched_words, "fancy_string"));
        fancy_string_array_destroy(matched_words);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(pattern);
        fancy_string_destroy(s);
    }
    // {
    //     // NOTE: What is "invalid" appears to depend on the implementation, so this could
    //     // cause trouble, so I'm disabling it for now until I can further investigate...
    //     char *invalid_patterns[3] = {"[a-z]{3,2,1}", "[a-z[", "[]"};
    //     for (size_t i = 0; i < 3; i++)
    //     {
    //         fancy_string_t *s = fancy_string_create("will use invalid regular expression pattern");
    //         fancy_string_t *pattern = fancy_string_create(invalid_patterns[i]);
    //         fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
    //         assert(re == NULL);
    //         fancy_string_destroy(pattern);
    //         fancy_string_destroy(s);
    //     }
    // }
    {
        fancy_string_t *s = fancy_string_create("will use empty pattern");
        fancy_string_t *pattern = fancy_string_create("");
        fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
        assert(re == NULL);
        fancy_string_destroy(pattern);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("");
        fancy_string_t *pattern = fancy_string_create(".*");
        fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
        assert(re == NULL);
        fancy_string_destroy(pattern);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("I will try to match all.");
        fancy_string_t *pattern = fancy_string_create(".*");
        fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
        assert(re != NULL);
        assert(fancy_string_regex_match_count(re) == 1);
        {
            fancy_string_array_t *a = fancy_string_regex_matches_to_strings(re);
            assert(fancy_string_array_size(a) == 1);
            fancy_string_t *s_1 = fancy_string_array_shift(a);
            assert(fancy_string_equals(s_1, s));
            fancy_string_destroy(s_1);
            fancy_string_array_destroy(a);
        }
        fancy_string_regex_destroy(re);
        fancy_string_destroy(pattern);
        fancy_string_destroy(s);
    }
}

struct test_fancy_string_regex_to_string_with_updated_matches_updater_callback_context
{
    char *uppercase_if_equals_to;
};

static void test_fancy_string_regex_to_string_with_updated_matches_updater_callback(fancy_string_t *const match, size_t start, size_t end, fancy_string_t const *const string, void *context)
{
    IGNORE_UNUSED(start);
    IGNORE_UNUSED(end);
    IGNORE_UNUSED(string);

    struct test_fancy_string_regex_to_string_with_updated_matches_updater_callback_context *c = (struct test_fancy_string_regex_to_string_with_updated_matches_updater_callback_context *)context;

    if (fancy_string_equals_value(match, c->uppercase_if_equals_to))
    {
        fancy_string_uppercase(match);
    }

    fancy_string_prepend_value(match, "[[");
    fancy_string_append_value(match, "]]");
}

void test_fancy_string_regex_to_string_with_updated_matches(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("My name is Francis and I wrote this library for fun!");
    fancy_string_t *pattern = fancy_string_create("[A-Z][a-z]*");
    fancy_string_regex_t *re = fancy_string_regex_create(s, pattern, -1);
    assert(re != NULL);
    struct test_fancy_string_regex_to_string_with_updated_matches_updater_callback_context context = {.uppercase_if_equals_to = "Francis"};
    fancy_string_t *updated_string = fancy_string_regex_to_string_with_updated_matches(re, test_fancy_string_regex_to_string_with_updated_matches_updater_callback, &context);
    assert(fancy_string_equals_value(updated_string, "[[My]] name is [[FRANCIS]] and [[I]] wrote this library for fun!"));
    fancy_string_destroy(updated_string);
    fancy_string_regex_destroy(re);
    fancy_string_destroy(pattern);
    fancy_string_destroy(s);
}

void test_fancy_string_regex_destroy(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("My name is Francis and I wrote this program.");
        fancy_string_t *p = fancy_string_create("[A-Z][a-z]*");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_debug(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("My name is Francis and I wrote this program.");
        fancy_string_t *p = fancy_string_create("[A-Z][a-z]*");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);

        size_t buffer_size = 1000;

        char *buffer_verbose_false = malloc(sizeof(char) * buffer_size);
        FILE *verbose_false;
        verbose_false = fmemopen(buffer_verbose_false, buffer_size, "w+");
        assert(verbose_false != NULL);
        setvbuf(verbose_false, NULL, _IONBF, 0);
        fancy_string_regex_debug(re, verbose_false, false);
        assert(strlen(buffer_verbose_false) > 0);

        char *buffer_verbose_true = malloc(sizeof(char) * buffer_size);
        FILE *verbose_true;
        verbose_true = fmemopen(buffer_verbose_true, buffer_size, "w+");
        assert(verbose_true != NULL);
        setvbuf(verbose_true, NULL, _IONBF, 0);
        fancy_string_regex_debug(re, verbose_true, true);
        assert(strlen(buffer_verbose_true) > strlen(buffer_verbose_false));

        fclose(verbose_false);
        free(buffer_verbose_false);
        fclose(verbose_true);
        free(buffer_verbose_true);

        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_matches_to_strings(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("My name is Francis and I wrote this program.");
        fancy_string_t *p = fancy_string_create("[A-Z][a-z]*");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        fancy_string_array_t *matched_words = fancy_string_regex_matches_to_strings(re);
        assert(fancy_string_array_size(matched_words) == 3);
        char *words[3] = {
            "My",
            "Francis",
            "I",
        };
        for (size_t i = 0; i < 3; i++)
        {
            fancy_string_t *tmp = fancy_string_array_get(matched_words, i);
            assert(fancy_string_equals_value(tmp, words[i]));
            fancy_string_destroy(tmp);
        }
        fancy_string_array_destroy(matched_words);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_replaced_matches(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("is this a test. this is a test is a test is is");
        fancy_string_t *p = fancy_string_create("(^is)|(is$)");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        fancy_string_t *replacement = fancy_string_create("IS");
        fancy_string_t *new_string = fancy_string_regex_replaced_matches(re, replacement);
        assert(fancy_string_equals_value(new_string, "IS this a test. this is a test is a test is IS"));
        fancy_string_destroy(new_string);
        fancy_string_destroy(replacement);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("is this a test. this is a test is a test is is");
        fancy_string_t *p = fancy_string_create("is");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        fancy_string_t *replacement = fancy_string_create("IS");
        fancy_string_t *new_string = fancy_string_regex_replaced_matches(re, replacement);
        assert(fancy_string_equals_value(new_string, "IS thIS a test. thIS IS a test IS a test IS IS"));
        fancy_string_destroy(new_string);
        fancy_string_destroy(replacement);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("is this a test. this is a test is a test is is");
        fancy_string_t *p = fancy_string_create("is");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 2);
        fancy_string_t *replacement = fancy_string_create("IS");
        fancy_string_t *new_string = fancy_string_regex_replaced_matches(re, replacement);
        assert(fancy_string_equals_value(new_string, "IS thIS a test. this is a test is a test is is"));
        fancy_string_destroy(new_string);
        fancy_string_destroy(replacement);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_split_at_matches(void)
{
    LOG();
    size_t max_values[8] = {-1, 0, 1, 2, 3, 4, 5, 10};
    for (size_t i = 0; i < 8; i++)
    {
        fancy_string_t *s = fancy_string_create("is this a test. this is a test is a test is is");
        fancy_string_t *p = fancy_string_create("is");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, max_values[i]);
        fancy_string_array_t *a = fancy_string_regex_split_at_matches(re);
        assert(fancy_string_regex_match_count(re) + 1 == fancy_string_array_size(a));
        fancy_string_array_destroy(a);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_match_count(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 2);
        assert(fancy_string_regex_match_count(re) == 2);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 1);
        assert(fancy_string_regex_match_count(re) == 1);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        assert(fancy_string_regex_match_count(re) == 2);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 0);
        assert(fancy_string_regex_match_count(re) == 0);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_has_match(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 0);
        assert(!fancy_string_regex_has_match(re));
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        assert(fancy_string_regex_has_match(re));
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("tester");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        assert(!fancy_string_regex_has_match(re));
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_max_number_of_matches(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 3);
        assert(fancy_string_regex_max_number_of_matches(re) == 3);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        assert(fancy_string_regex_max_number_of_matches(re) == -1);
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_max_number_of_matches_reached(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 3);
        assert(!fancy_string_regex_max_number_of_matches_reached(re));
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 1);
        assert(fancy_string_regex_max_number_of_matches_reached(re));
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, 2);
        assert(fancy_string_regex_max_number_of_matches_reached(re));
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("this is a test is a test");
        fancy_string_t *p = fancy_string_create("test");
        fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
        assert(!fancy_string_regex_max_number_of_matches_reached(re));
        fancy_string_regex_destroy(re);
        fancy_string_destroy(p);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_regex_string(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("this is a test");
    fancy_string_t *p = fancy_string_create("test$");
    fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
    fancy_string_t *string = fancy_string_regex_string(re);
    assert(fancy_string_equals(string, s));
    fancy_string_destroy(string);
    fancy_string_regex_destroy(re);
    fancy_string_destroy(p);
    fancy_string_destroy(s);
}

void test_fancy_string_regex_pattern(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("this is a test");
    fancy_string_t *p = fancy_string_create("test$");
    fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
    fancy_string_t *pattern = fancy_string_regex_pattern(re);
    assert(fancy_string_equals(pattern, p));
    fancy_string_destroy(pattern);
    fancy_string_regex_destroy(re);
    fancy_string_destroy(p);
    fancy_string_destroy(s);
}

void test_fancy_string_regex_match_info_for_index(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("jules julie julius jean jeffrey joey john joe");
    fancy_string_t *p = fancy_string_create("j[a-z]*e($|[ ]{1})");
    fancy_string_regex_t *re = fancy_string_regex_create(s, p, -1);
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
}

void test_fancy_string_array_includes_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin justin julie jeffry jeffster john jordan jacob jean joe joey joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    assert(!fancy_string_array_includes_value(a, "francis"));
    assert(fancy_string_array_includes_value(a, "joey"));
    assert(fancy_string_array_includes_value(a, "justin"));
    assert(fancy_string_array_includes_value(a, "jeffry"));
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_includes(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin justin julie jeffry jeffster john jordan jacob jean joe joey joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        fancy_string_t *s_2 = fancy_string_create("francis");
        assert(!fancy_string_array_includes(a, s_2));
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("joey");
        assert(fancy_string_array_includes(a, s_2));
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("justin");
        assert(fancy_string_array_includes(a, s_2));
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("jeffry");
        assert(fancy_string_array_includes(a, s_2));
        fancy_string_destroy(s_2);
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_last_index_of_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin justin julie jeffry jeffster john jordan jacob jean joe joey joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    assert(fancy_string_array_last_index_of_value(a, "francis") == -1);
    assert(fancy_string_array_last_index_of_value(a, "joey") == 11);
    assert(fancy_string_array_last_index_of_value(a, "justin") == 1);
    assert(fancy_string_array_last_index_of_value(a, "jeffry") == 3);
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_last_index_of(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin justin julie jeffry jeffster john jordan jacob jean joe joey joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        fancy_string_t *s_2 = fancy_string_create("francis");
        assert(fancy_string_array_last_index_of(a, s_2) == -1);
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("joey");
        assert(fancy_string_array_last_index_of(a, s_2) == 11);
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("justin");
        assert(fancy_string_array_last_index_of(a, s_2) == 1);
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("jeffry");
        assert(fancy_string_array_last_index_of(a, s_2) == 3);
        fancy_string_destroy(s_2);
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_index_of_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin justin julie jeffry jeffster john jordan jacob jean joe joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    assert(fancy_string_array_index_of_value(a, "francis") == -1);
    assert(fancy_string_array_index_of_value(a, "joey") == 10);
    assert(fancy_string_array_index_of_value(a, "justin") == 0);
    assert(fancy_string_array_index_of_value(a, "jeffry") == 3);
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_index_of(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin justin julie jeffry jeffster john jordan jacob jean joe joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        fancy_string_t *s_2 = fancy_string_create("francis");
        assert(fancy_string_array_index_of(a, s_2) == -1);
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("joey");
        assert(fancy_string_array_index_of(a, s_2) == 10);
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("justin");
        assert(fancy_string_array_index_of(a, s_2) == 0);
        fancy_string_destroy(s_2);
    }
    {
        fancy_string_t *s_2 = fancy_string_create("jeffry");
        assert(fancy_string_array_index_of(a, s_2) == 3);
        fancy_string_destroy(s_2);
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

static bool
test_fancy_string_array_some_callback_1(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(index);
    IGNORE_UNUSED(array);

    char *starts_with = (char *)context;
    return fancy_string_starts_with_value(string, starts_with);
}

void test_fancy_string_array_every(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin julie jeffry jeffster john jordan jacob jean joe joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        char *starts_with = "ju";
        assert(!fancy_string_array_every(a, test_fancy_string_array_some_callback_1, starts_with));
    }
    {
        char *starts_with = "al";
        assert(!fancy_string_array_every(a, test_fancy_string_array_some_callback_1, starts_with));
    }
    {
        char *starts_with = "j";
        assert(fancy_string_array_every(a, test_fancy_string_array_some_callback_1, starts_with));
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_none(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin julie jeffry jeffster john jordan jacob jean joe joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        char *starts_with = "ju";
        assert(!fancy_string_array_none(a, test_fancy_string_array_some_callback_1, starts_with));
    }
    {
        char *starts_with = "al";
        assert(fancy_string_array_none(a, test_fancy_string_array_some_callback_1, starts_with));
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_some(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("justin julie jeffry jeffster john jordan jacob jean joe joey");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        char *starts_with = "ju";
        assert(fancy_string_array_some(a, test_fancy_string_array_some_callback_1, starts_with));
    }
    {
        char *starts_with = "al";
        assert(!fancy_string_array_some(a, test_fancy_string_array_some_callback_1, starts_with));
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

struct test_fancy_string_array_find_last_index_callback_1_context
{
    size_t index_must_be_lesser_than_or_equal_to;
    char const *const value_starts_with;
};

static bool
test_fancy_string_array_find_last_index_callback_1(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(array);

    struct test_fancy_string_array_find_last_index_callback_1_context *info = context;

    if (index > info->index_must_be_lesser_than_or_equal_to)
    {
        return false;
    }

    return fancy_string_starts_with_value(string, info->value_starts_with);
}

static bool
test_fancy_string_array_find_last_index_callback_2(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(string);
    IGNORE_UNUSED(index);
    IGNORE_UNUSED(array);
    IGNORE_UNUSED(context);

    return true;
}

static bool
test_fancy_string_array_find_last_index_callback_3(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(string);
    IGNORE_UNUSED(array);

    size_t context_index = *((size_t *)context);
    return context_index == index ? true : false;
}

void test_fancy_string_array_find_last(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test. This is just a test. This is only a simple test.");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        struct test_fancy_string_array_find_last_index_callback_1_context context = {
            .index_must_be_lesser_than_or_equal_to = 1,
            .value_starts_with = "won_t_be_matched"};
        fancy_string_t *last = fancy_string_array_find_last(a, test_fancy_string_array_find_last_index_callback_1, &context);
        assert(last == NULL);
    }
    {
        struct test_fancy_string_array_find_last_index_callback_1_context context = {
            .index_must_be_lesser_than_or_equal_to = 20,
            .value_starts_with = "Th"};
        fancy_string_t *last = fancy_string_array_find_last(a, test_fancy_string_array_find_last_index_callback_1, &context);
        assert(fancy_string_equals_value(last, "This"));
        fancy_string_destroy(last);
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_find_last_index(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test. This is just a test. This is only a simple test.");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        size_t n = fancy_string_array_size(a);
        for (size_t context_index = 0; context_index < n; context_index++)
        {
            ssize_t index = fancy_string_array_find_last_index(a, test_fancy_string_array_find_last_index_callback_3, &context_index);
            assert(index != -1);
            assert((size_t)index == context_index);
        }
        ssize_t index = fancy_string_array_find_last_index(a, test_fancy_string_array_find_last_index_callback_3, &n);
        assert(index == -1);
    }
    {
        ssize_t index = fancy_string_array_find_last_index(a, test_fancy_string_array_find_last_index_callback_2, NULL);
        assert(index != -1);
        assert(fancy_string_array_size(a) > 0);
        assert((size_t)index == fancy_string_array_size(a) - 1);
    }
    {
        struct test_fancy_string_array_find_last_index_callback_1_context context = {
            .index_must_be_lesser_than_or_equal_to = 1,
            .value_starts_with = "won_t_be_matched"};
        ssize_t index = fancy_string_array_find_last_index(a, test_fancy_string_array_find_last_index_callback_1, &context);
        assert(index == -1);
    }
    {
        struct test_fancy_string_array_find_last_index_callback_1_context context = {
            .index_must_be_lesser_than_or_equal_to = 20,
            .value_starts_with = "Th"};
        ssize_t index = fancy_string_array_find_last_index(a, test_fancy_string_array_find_last_index_callback_1, &context);
        assert(index == 9);
    }
    {
        struct test_fancy_string_array_find_last_index_callback_1_context context = {
            .index_must_be_lesser_than_or_equal_to = 4,
            .value_starts_with = "tes"};
        ssize_t index = fancy_string_array_find_last_index(a, test_fancy_string_array_find_last_index_callback_1, &context);
        assert(index == 3);
    }
    {
        struct test_fancy_string_array_find_last_index_callback_1_context context = {
            .index_must_be_lesser_than_or_equal_to = 100,
            .value_starts_with = "tes"};
        ssize_t index = fancy_string_array_find_last_index(a, test_fancy_string_array_find_last_index_callback_1, &context);
        assert(index == 14);
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

struct test_fancy_string_array_find_index_callback_1_context
{
    size_t index_must_be_greater_than_or_equal_to;
    char const *const value_starts_with;
};

static bool
test_fancy_string_array_find_index_callback_1(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(array);

    struct test_fancy_string_array_find_index_callback_1_context *info = context;

    if (index < info->index_must_be_greater_than_or_equal_to)
    {
        return false;
    }

    return fancy_string_starts_with_value(string, info->value_starts_with);
}

void test_fancy_string_array_find(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test. This is just a test. This is only a simple test.");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        struct test_fancy_string_array_find_index_callback_1_context context = {
            .index_must_be_greater_than_or_equal_to = 1,
            .value_starts_with = "Th"};
        fancy_string_t *string = fancy_string_array_find(a, test_fancy_string_array_find_index_callback_1, &context);
        assert(string != NULL);
        assert(fancy_string_equals_value(string, "This"));
        fancy_string_destroy(string);
    }
    {
        struct test_fancy_string_array_find_index_callback_1_context context = {
            .index_must_be_greater_than_or_equal_to = 1,
            .value_starts_with = "NoMatch"};
        fancy_string_t *string = fancy_string_array_find(a, test_fancy_string_array_find_index_callback_1, &context);
        assert(string == NULL);
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_find_index(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test. This is just a test. This is only a simple test.");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        struct test_fancy_string_array_find_index_callback_1_context context = {
            .index_must_be_greater_than_or_equal_to = 1,
            .value_starts_with = "won_t_be_matched"};
        ssize_t index = fancy_string_array_find_index(a, test_fancy_string_array_find_index_callback_1, &context);
        assert(index == -1);
    }
    {
        struct test_fancy_string_array_find_index_callback_1_context context = {
            .index_must_be_greater_than_or_equal_to = 1,
            .value_starts_with = "Th"};
        ssize_t index = fancy_string_array_find_index(a, test_fancy_string_array_find_index_callback_1, &context);
        assert(index == 4);
    }
    {
        struct test_fancy_string_array_find_index_callback_1_context context = {
            .index_must_be_greater_than_or_equal_to = 4,
            .value_starts_with = "tes"};
        ssize_t index = fancy_string_array_find_index(a, test_fancy_string_array_find_index_callback_1, &context);
        assert(index == 8);
    }
    {
        struct test_fancy_string_array_find_index_callback_1_context context = {
            .index_must_be_greater_than_or_equal_to = 3,
            .value_starts_with = "tes"};
        ssize_t index = fancy_string_array_find_index(a, test_fancy_string_array_find_index_callback_1, &context);
        assert(index == 3);
    }
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_spliced(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "a");
    fancy_string_array_push_value(a, "b");
    fancy_string_array_push_value(a, "c");
    fancy_string_array_push_value(a, "d");
    fancy_string_array_push_value(a, "e");
    fancy_string_array_t *spliced = fancy_string_array_spliced(a, 1, -1, NULL);
    assert(fancy_string_array_size(a) == 5);
    assert(fancy_string_array_size(spliced) == 1);
    fancy_string_t *first = fancy_string_array_first(spliced);
    assert(fancy_string_equals_value(first, "a"));
    fancy_string_destroy(first);
    fancy_string_array_destroy(spliced);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_splice_and_destroy(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "a");
    fancy_string_array_push_value(a, "b");
    fancy_string_array_push_value(a, "c");
    fancy_string_array_push_value(a, "d");
    fancy_string_array_push_value(a, "e");
    fancy_string_array_splice_and_destroy(a, 1, -1, NULL);
    assert(fancy_string_array_size(a) == 1);
    fancy_string_t *first = fancy_string_array_first(a);
    assert(fancy_string_equals_value(first, "a"));
    fancy_string_destroy(first);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_splice(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "a");
    fancy_string_array_push_value(a, "b");
    fancy_string_array_push_value(a, "c");
    fancy_string_array_push_value(a, "d");
    fancy_string_array_push_value(a, "e");
    {
        fancy_string_array_t *new_items = fancy_string_array_create();
        fancy_string_array_push_value(new_items, "A");
        fancy_string_array_push_value(new_items, "B");
        fancy_string_array_push_value(new_items, "C");
        fancy_string_array_t *cloned = fancy_string_array_clone(a);
        fancy_string_array_t *removed_items = fancy_string_array_splice(cloned, 0, -1, new_items);
        assert(fancy_string_array_size(removed_items) == 5);
        {
            fancy_string_t *removed_joined = fancy_string_array_join_by_value(removed_items, ",");
            assert(fancy_string_equals_value(removed_joined, "a,b,c,d,e"));
            fancy_string_destroy(removed_joined);
        }
        assert(fancy_string_array_size(cloned) == 3);
        fancy_string_t *joined = fancy_string_array_join_by_value(cloned, ",");
        assert(fancy_string_equals_value(joined, "A,B,C"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(removed_items);
        fancy_string_array_destroy(cloned);
        fancy_string_array_destroy(new_items);
    }
    {
        fancy_string_array_t *new_items = fancy_string_array_create();
        fancy_string_array_push_value(new_items, "A");
        fancy_string_array_push_value(new_items, "B");
        fancy_string_array_push_value(new_items, "C");
        fancy_string_array_t *cloned = fancy_string_array_clone(a);
        fancy_string_array_t *removed_items = fancy_string_array_splice(cloned, 5, 1, new_items);
        assert(fancy_string_array_size(removed_items) == 0);
        assert(fancy_string_array_size(cloned) == 8);
        fancy_string_t *joined = fancy_string_array_join_by_value(cloned, ",");
        assert(fancy_string_equals_value(joined, "a,b,c,d,e,A,B,C"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(removed_items);
        fancy_string_array_destroy(cloned);
        fancy_string_array_destroy(new_items);
    }
    {
        fancy_string_array_t *new_items = fancy_string_array_create();
        fancy_string_array_push_value(new_items, "A");
        fancy_string_array_push_value(new_items, "B");
        fancy_string_array_push_value(new_items, "C");
        fancy_string_array_t *cloned = fancy_string_array_clone(a);
        fancy_string_array_t *removed_items = fancy_string_array_splice(cloned, 3, 1, new_items);
        assert(fancy_string_array_size(removed_items) == 1);
        {
            fancy_string_t *removed_joined = fancy_string_array_join_by_value(removed_items, ",");
            assert(fancy_string_equals_value(removed_joined, "d"));
            fancy_string_destroy(removed_joined);
        }
        assert(fancy_string_array_size(cloned) == 7);
        fancy_string_t *joined = fancy_string_array_join_by_value(cloned, ",");
        assert(fancy_string_equals_value(joined, "a,b,c,A,B,C,e"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(removed_items);
        fancy_string_array_destroy(cloned);
        fancy_string_array_destroy(new_items);
    }
    {
        fancy_string_array_t *new_items = fancy_string_array_create();
        fancy_string_array_push_value(new_items, "A");
        fancy_string_array_push_value(new_items, "B");
        fancy_string_array_push_value(new_items, "C");
        fancy_string_array_t *cloned = fancy_string_array_clone(a);
        fancy_string_array_t *removed_items = fancy_string_array_splice(cloned, 0, 2, new_items);
        assert(fancy_string_array_size(removed_items) == 2);
        {
            fancy_string_t *removed_joined = fancy_string_array_join_by_value(removed_items, ",");
            assert(fancy_string_equals_value(removed_joined, "a,b"));
            fancy_string_destroy(removed_joined);
        }
        assert(fancy_string_array_size(cloned) == 6);
        fancy_string_t *joined = fancy_string_array_join_by_value(cloned, ",");
        assert(fancy_string_equals_value(joined, "A,B,C,c,d,e"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(removed_items);
        fancy_string_array_destroy(cloned);
        fancy_string_array_destroy(new_items);
    }
    {
        fancy_string_array_t *new_items = fancy_string_array_create();
        fancy_string_array_push_value(new_items, "A");
        fancy_string_array_push_value(new_items, "B");
        fancy_string_array_push_value(new_items, "C");
        fancy_string_array_t *cloned = fancy_string_array_clone(a);
        fancy_string_array_t *removed_items = fancy_string_array_splice(cloned, 0, 0, new_items);
        assert(fancy_string_array_is_empty(removed_items));
        assert(fancy_string_array_size(cloned) == 8);
        fancy_string_t *joined = fancy_string_array_join_by_value(cloned, ",");
        assert(fancy_string_equals_value(joined, "A,B,C,a,b,c,d,e"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(removed_items);
        fancy_string_array_destroy(cloned);
        fancy_string_array_destroy(new_items);
    }
    {
        fancy_string_array_t *cloned = fancy_string_array_clone(a);
        fancy_string_array_t *removed_items = fancy_string_array_splice(cloned, 10, -1, NULL);
        assert(fancy_string_array_is_empty(removed_items));
        assert(fancy_string_array_size(cloned) == 5);
        fancy_string_array_destroy(removed_items);
        fancy_string_array_destroy(cloned);
    }
    {
        fancy_string_array_t *cloned = fancy_string_array_clone(a);
        fancy_string_array_t *removed_items = fancy_string_array_splice(cloned, 0, -1, NULL);
        assert(fancy_string_array_is_empty(cloned));
        assert(fancy_string_array_size(removed_items) == 5);
        fancy_string_array_destroy(removed_items);
        fancy_string_array_destroy(cloned);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_slice_and_destroy(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("0 1 2 3 4 5 6 7 8 9");
    {
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
        fancy_string_array_slice_and_destroy(a, 0, 2);
        fancy_string_t *joined = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(joined, "0 1"));
        fancy_string_array_destroy(a);
        fancy_string_destroy(joined);
    }
    fancy_string_destroy(s);
}

void test_fancy_string_array_sliced(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("0 1 2 3 4 5 6 7 8 9");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 8, 9);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_equals_value(joined, "8"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 0, 9);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_equals_value(joined, "0 1 2 3 4 5 6 7 8"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 0, -1);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_equals(joined, s));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, -1, -1);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_equals(joined, s));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 0, 111);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_equals(joined, s));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 2, 4);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_equals_value(joined, "2 3"));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 9, -1);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, "9");
        assert(fancy_string_size(joined) == 1);
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 10, 11);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_is_empty(joined));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 12, 11);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_is_empty(joined));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 3, 1);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_is_empty(joined));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    {
        fancy_string_array_t *sliced = fancy_string_array_sliced(a, 1, 1);
        fancy_string_t *joined = fancy_string_array_join_by_value(sliced, " ");
        assert(fancy_string_is_empty(joined));
        fancy_string_destroy(joined);
        fancy_string_array_destroy(sliced);
    }
    fancy_string_destroy(s);
    fancy_string_array_destroy(a);
}

static bool test_fancy_string_array_filter_callback(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    IGNORE_UNUSED(index);
    IGNORE_UNUSED(array);

    char *cannot_start_with = (char *)context;

    return !fancy_string_starts_with_value(string, cannot_start_with);
}

void test_fancy_string_array_filtered(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "john");
    fancy_string_array_push_value(a, "jerry");
    fancy_string_array_push_value(a, "jimmy");
    fancy_string_array_push_value(a, "george");
    fancy_string_array_push_value(a, "alberto");
    fancy_string_array_push_value(a, "france");
    fancy_string_array_push_value(a, "zelda");
    char *cannot_start_with = "j";
    fancy_string_array_t *filtered = fancy_string_array_filtered(a, test_fancy_string_array_filter_callback, cannot_start_with);
    fancy_string_t *joined = fancy_string_array_join_by_value(filtered, " ");
    assert(fancy_string_equals_value(joined, "george alberto france zelda"));
    fancy_string_destroy(joined);
    fancy_string_array_destroy(filtered);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_filter(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "john");
    fancy_string_array_push_value(a, "jerry");
    fancy_string_array_push_value(a, "jimmy");
    fancy_string_array_push_value(a, "george");
    fancy_string_array_push_value(a, "alberto");
    fancy_string_array_push_value(a, "france");
    fancy_string_array_push_value(a, "zelda");
    char *cannot_start_with = "j";
    fancy_string_array_filter(a, test_fancy_string_array_filter_callback, cannot_start_with);
    fancy_string_t *joined = fancy_string_array_join_by_value(a, " ");
    assert(fancy_string_equals_value(joined, "george alberto france zelda"));
    fancy_string_destroy(joined);
    fancy_string_array_destroy(a);
}

static bool test_fancy_string_array_sort_callback_1(fancy_string_t const *const string_1, fancy_string_t const *const string_2, void *context)
{
    IGNORE_UNUSED(context);

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

void test_fancy_string_array_sorted(void)
{
    LOG();
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "nick");
        fancy_string_array_push_value(a, "jimmy");
        fancy_string_array_push_value(a, "jeffrey");
        fancy_string_array_push_value(a, "jim");
        fancy_string_array_push_value(a, "john");
        fancy_string_array_push_value(a, "julie");
        fancy_string_array_push_value(a, "stephane");
        fancy_string_array_push_value(a, "lauren");
        fancy_string_array_push_value(a, "isaac");
        fancy_string_array_push_value(a, "alberto");
        fancy_string_array_push_value(a, "rob");
        fancy_string_array_push_value(a, "bob");
        fancy_string_array_t *sorted = fancy_string_array_sorted(a, test_fancy_string_array_sort_callback_1, NULL);
        fancy_string_t *s = fancy_string_array_join_by_value(sorted, " ");
        assert(fancy_string_equals_value(s, "alberto bob isaac jeffrey jim jimmy john julie lauren nick rob stephane"));
        fancy_string_destroy(s);
        fancy_string_array_destroy(sorted);
        fancy_string_array_destroy(a);
    }
}

void test_fancy_string_array_sort(void)
{
    LOG();
    // More testing needed
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "nick");
        fancy_string_array_push_value(a, "jimmy");
        fancy_string_array_push_value(a, "jeffrey");
        fancy_string_array_push_value(a, "jim");
        fancy_string_array_push_value(a, "john");
        fancy_string_array_push_value(a, "julie");
        fancy_string_array_push_value(a, "stephane");
        fancy_string_array_push_value(a, "lauren");
        fancy_string_array_push_value(a, "isaac");
        fancy_string_array_push_value(a, "alberto");
        fancy_string_array_push_value(a, "rob");
        fancy_string_array_push_value(a, "bob");
        fancy_string_array_sort(a, test_fancy_string_array_sort_callback_1, NULL);
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "alberto bob isaac jeffrey jim jimmy john julie lauren nick rob stephane"));
        // fancy_string_array_print(a, stdout, true);
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "b");
        fancy_string_array_push_value(a, "d");
        fancy_string_array_push_value(a, "c");
        fancy_string_array_push_value(a, "a");
        fancy_string_array_push_value(a, "g");
        fancy_string_array_push_value(a, "e");
        fancy_string_array_push_value(a, "h");
        fancy_string_array_push_value(a, "f");
        fancy_string_array_sort(a, test_fancy_string_array_sort_callback_1, NULL);
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "a b c d e f g h"));
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
}

void test_fancy_string_array_sorted_values(void)
{
    LOG();
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "b");
        fancy_string_array_push_value(a, "d");
        fancy_string_array_push_value(a, "c");
        fancy_string_array_push_value(a, "a");
        fancy_string_array_push_value(a, "g");
        fancy_string_array_push_value(a, "e");
        fancy_string_array_push_value(a, "h");
        fancy_string_array_push_value(a, "f");
        fancy_string_array_sort_values(a);
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "a b c d e f g h"));
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "bob");
        fancy_string_array_push_value(a, "antoine");
        fancy_string_array_push_value(a, "bobby");
        fancy_string_array_push_value(a, "anna");
        fancy_string_array_push_value(a, "ann");
        fancy_string_array_sort_values(a);
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "ann anna antoine bob bobby"));
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
}

void test_fancy_string_array_sort_values(void)
{
    LOG();
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "b");
        fancy_string_array_push_value(a, "d");
        fancy_string_array_push_value(a, "c");
        fancy_string_array_push_value(a, "a");
        fancy_string_array_push_value(a, "g");
        fancy_string_array_push_value(a, "e");
        fancy_string_array_push_value(a, "h");
        fancy_string_array_push_value(a, "f");
        fancy_string_array_t *a_sorted = fancy_string_array_sorted_values(a);
        fancy_string_t *s = fancy_string_array_join_by_value(a_sorted, " ");
        assert(fancy_string_equals_value(s, "a b c d e f g h"));
        fancy_string_array_destroy(a_sorted);
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "bob");
        fancy_string_array_push_value(a, "antoine");
        fancy_string_array_push_value(a, "bobby");
        fancy_string_array_push_value(a, "anna");
        fancy_string_array_push_value(a, "ann");
        fancy_string_array_t *a_sorted = fancy_string_array_sorted_values(a);
        fancy_string_t *s = fancy_string_array_join_by_value(a_sorted, " ");
        assert(fancy_string_equals_value(s, "ann anna antoine bob bobby"));
        fancy_string_array_destroy(a_sorted);
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
}

static fancy_string_t *test_fancy_string_array_mapped_callback(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    char *prefix = (char *)context;
    fancy_string_t *tmp = fancy_string_array_get(array, index);
    assert(fancy_string_equals(string, tmp));
    ssize_t tmp_i = fancy_string_array_index_of(array, string);
    assert((size_t)tmp_i == index);
    fancy_string_destroy(tmp);
    fancy_string_t *modified = fancy_string_clone(string); // Could have used tmp, but normally we would not fetch it like that, so this better conveys how this function should be used.
    fancy_string_prepend_value(modified, prefix);
    return modified;
}

void test_fancy_string_array_mapped(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    char *context = "value ";
    fancy_string_array_t *mapped = fancy_string_array_mapped(a, test_fancy_string_array_mapped_callback, context);
    fancy_string_t *s = fancy_string_array_join_by_value(mapped, "; ");
    assert(fancy_string_equals_value(s, "value one; value two; value three"));
    fancy_string_destroy(s);
    fancy_string_array_destroy(mapped);
    fancy_string_array_destroy(a);
}

static void test_fancy_string_array_for_each_callback(fancy_string_t *const string, size_t index, fancy_string_array_t const *const array, void *context)
{
    char *prefix = (char *)context;
    fancy_string_t *tmp = fancy_string_array_get(array, index);
    assert(fancy_string_equals(string, tmp));
    ssize_t tmp_i = fancy_string_array_index_of(array, string);
    assert((size_t)tmp_i == index);
    fancy_string_destroy(tmp);
    fancy_string_prepend_value(string, prefix);
}

void test_fancy_string_array_for_each(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    char *context = "value ";
    fancy_string_array_for_each(a, test_fancy_string_array_for_each_callback, context);
    fancy_string_t *s = fancy_string_array_join_by_value(a, "; ");
    assert(fancy_string_equals_value(s, "value one; value two; value three"));
    fancy_string_destroy(s);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_reversed(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    {
        fancy_string_array_t *reversed = fancy_string_array_reversed(a);
        fancy_string_t *s = fancy_string_array_join_by_value(reversed, " ");
        assert(fancy_string_equals_value(s, "four three two one"));
        fancy_string_destroy(s);
        fancy_string_array_destroy(reversed);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_reverse(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "one two three four"));
        fancy_string_destroy(s);
    }
    fancy_string_array_reverse(a);
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "four three two one"));
        fancy_string_destroy(s);
    }
    fancy_string_array_reverse(a);
    fancy_string_array_push_value(a, "five");
    fancy_string_array_reverse(a);
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "five four three two one"));
        fancy_string_destroy(s);
    }
    fancy_string_array_clear(a);
    fancy_string_array_push_value(a, "one");
    fancy_string_array_reverse(a);
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "one"));
        fancy_string_destroy(s);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_join(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    {
        fancy_string_t *separator = fancy_string_create("_-_");
        fancy_string_t *s = fancy_string_array_join(a, separator);
        assert(fancy_string_equals_value(s, "one_-_two_-_three_-_four"));
        fancy_string_destroy(separator);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *separator = fancy_string_create(" ");
        fancy_string_t *s = fancy_string_array_join(a, separator);
        assert(fancy_string_equals_value(s, "one two three four"));
        fancy_string_destroy(separator);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *separator = fancy_string_create_empty();
        fancy_string_t *s = fancy_string_array_join(a, separator);
        assert(fancy_string_equals_value(s, "onetwothreefour"));
        fancy_string_destroy(separator);
        fancy_string_destroy(s);
    }
    fancy_string_array_clear(a);
    {
        fancy_string_t *separator = fancy_string_create("hey");
        fancy_string_t *s = fancy_string_array_join(a, separator);
        assert(fancy_string_equals_value(s, ""));
        fancy_string_destroy(separator);
        fancy_string_destroy(s);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_join_by_value(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, "_-_");
        assert(fancy_string_equals_value(s, "one_-_two_-_three_-_four"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "one two three four"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, "");
        assert(fancy_string_equals_value(s, "onetwothreefour"));
        fancy_string_destroy(s);
    }
    fancy_string_array_clear(a);
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, "hey");
        assert(fancy_string_equals_value(s, ""));
        fancy_string_destroy(s);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_last_value(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    {
        char *value = fancy_string_array_last_value(a);
        fancy_string_t *s = fancy_string_create(value);
        free(value);
        assert(fancy_string_equals_value(s, "three"));
        fancy_string_destroy(s);
    }
    fancy_string_array_reverse(a);
    {
        char *value = fancy_string_array_last_value(a);
        fancy_string_t *s = fancy_string_create(value);
        free(value);
        assert(fancy_string_equals_value(s, "one"));
        fancy_string_destroy(s);
    }
    fancy_string_array_pop_and_destroy(a);
    {
        char *value = fancy_string_array_last_value(a);
        fancy_string_t *s = fancy_string_create(value);
        free(value);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    fancy_string_array_clear(a);
    assert(fancy_string_array_last_value(a) == NULL);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_last(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    {
        fancy_string_t *s = fancy_string_array_last(a);
        assert(fancy_string_equals_value(s, "three"));
        fancy_string_destroy(s);
    }
    fancy_string_array_reverse(a);
    {
        fancy_string_t *s = fancy_string_array_last(a);
        assert(fancy_string_equals_value(s, "one"));
        fancy_string_destroy(s);
    }
    fancy_string_array_pop_and_destroy(a);
    {
        fancy_string_t *s = fancy_string_array_last(a);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    fancy_string_array_clear(a);
    assert(fancy_string_array_last(a) == NULL);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_first_value(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    {
        char *value = fancy_string_array_first_value(a);
        fancy_string_t *s = fancy_string_create(value);
        free(value);
        assert(fancy_string_equals_value(s, "one"));
        fancy_string_destroy(s);
    }
    fancy_string_array_reverse(a);
    {
        char *value = fancy_string_array_first_value(a);
        fancy_string_t *s = fancy_string_create(value);
        free(value);
        assert(fancy_string_equals_value(s, "three"));
        fancy_string_destroy(s);
    }
    fancy_string_array_shift_and_destroy(a);
    {
        char *value = fancy_string_array_first_value(a);
        fancy_string_t *s = fancy_string_create(value);
        free(value);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    fancy_string_array_clear(a);
    assert(fancy_string_array_first_value(a) == NULL);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_first(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    {
        fancy_string_t *s = fancy_string_array_first(a);
        assert(fancy_string_equals_value(s, "one"));
        fancy_string_destroy(s);
    }
    fancy_string_array_reverse(a);
    {
        fancy_string_t *s = fancy_string_array_first(a);
        assert(fancy_string_equals_value(s, "three"));
        fancy_string_destroy(s);
    }
    fancy_string_array_shift_and_destroy(a);
    {
        fancy_string_t *s = fancy_string_array_first(a);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    fancy_string_array_clear(a);
    assert(fancy_string_array_first(a) == NULL);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_insert(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_t *s = fancy_string_create("v1");
    fancy_string_array_insert(a, s, 0);
    fancy_string_update_value(s, "v2");
    fancy_string_array_insert(a, s, 0);
    fancy_string_update_value(s, "v3");
    fancy_string_array_insert(a, s, 1);
    fancy_string_update_value(s, "v4");
    fancy_string_array_insert(a, s, 3);
    fancy_string_t *joined = fancy_string_array_join_by_value(a, " ");
    assert(fancy_string_equals_value(joined, "v2 v3 v1 v4"));
    fancy_string_destroy(joined);
    fancy_string_destroy(s);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_insert_value(void)
{
    LOG();
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "hello");
        fancy_string_array_insert_value(a, "world", 4);
        assert(fancy_string_array_size(a) == 5);
        for (size_t i = 1; i < 4; i++)
        {
            fancy_string_t *s = fancy_string_array_get(a, i);
            assert(fancy_string_is_empty(s));
            fancy_string_destroy(s);
        }
        fancy_string_array_destroy(a);
    }
    fancy_string_array_t *a = fancy_string_array_create();
    {
        fancy_string_array_insert_value(a, "v1", 0);
        assert(fancy_string_array_size(a) == 1);
        ssize_t index = fancy_string_array_index_of_value(a, "v1");
        assert(index == 0);
    }
    {
        fancy_string_array_insert_value(a, "v2", 0);
        assert(fancy_string_array_size(a) == 2);
        ssize_t index = fancy_string_array_index_of_value(a, "v2");
        assert(index == 0);
    }
    {
        fancy_string_array_insert_value(a, "v3", 1);
        assert(fancy_string_array_size(a) == 3);
        ssize_t index = fancy_string_array_index_of_value(a, "v3");
        assert(index == 1);
    }
    {
        fancy_string_array_insert_value(a, "v4", 3);
        assert(fancy_string_array_size(a) == 4);
        ssize_t index = fancy_string_array_index_of_value(a, "v4");
        assert(index == 3);
    }
    {
        ssize_t invalid_index = fancy_string_array_index_of_value(a, "v5");
        assert(invalid_index == -1);
    }
    {
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "v2 v3 v1 v4"));
        fancy_string_destroy(s);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_shift_and_destroy(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    assert(fancy_string_array_size(a) == 4);
    fancy_string_array_shift_and_destroy(a);
    assert(fancy_string_array_size(a) == 3);
    {
        fancy_string_t *s = fancy_string_array_get(a, 0);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    fancy_string_array_shift_and_destroy(a);
    assert(fancy_string_array_size(a) == 2);
    fancy_string_array_shift_and_destroy(a);
    assert(fancy_string_array_size(a) == 1);
    fancy_string_array_shift_and_destroy(a);
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_shift_and_destroy(a);
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_shift(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    fancy_string_t *s = fancy_string_array_shift(a);
    assert(fancy_string_equals_value(s, "one"));
    fancy_string_destroy(s);
    s = fancy_string_array_shift(a);
    assert(fancy_string_equals_value(s, "two"));
    fancy_string_destroy(s);
    s = fancy_string_array_shift(a);
    assert(fancy_string_equals_value(s, "three"));
    fancy_string_destroy(s);
    s = fancy_string_array_shift(a);
    assert(fancy_string_equals_value(s, "four"));
    fancy_string_destroy(s);
    s = fancy_string_array_shift(a);
    assert(s == NULL);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_pop_and_destroy(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    assert(fancy_string_array_size(a) == 4);
    fancy_string_array_pop_and_destroy(a);
    assert(fancy_string_array_size(a) == 3);
    {
        fancy_string_t *s = fancy_string_array_get(a, 2);
        assert(fancy_string_equals_value(s, "three"));
        fancy_string_destroy(s);
    }
    fancy_string_array_pop_and_destroy(a);
    assert(fancy_string_array_size(a) == 2);
    fancy_string_array_pop_and_destroy(a);
    assert(fancy_string_array_size(a) == 1);
    fancy_string_array_pop_and_destroy(a);
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_pop_and_destroy(a);
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_pop(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    fancy_string_t *s = fancy_string_array_pop(a);
    assert(fancy_string_equals_value(s, "four"));
    fancy_string_destroy(s);
    s = fancy_string_array_pop(a);
    assert(fancy_string_equals_value(s, "three"));
    fancy_string_destroy(s);
    s = fancy_string_array_pop(a);
    assert(fancy_string_equals_value(s, "two"));
    fancy_string_destroy(s);
    s = fancy_string_array_pop(a);
    assert(fancy_string_equals_value(s, "one"));
    fancy_string_destroy(s);
    s = fancy_string_array_pop(a);
    assert(s == NULL);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_remove_and_destroy(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    assert(fancy_string_array_size(a) == 1);
    fancy_string_array_remove_and_destroy(a, 0);
    assert(fancy_string_array_size(a) == 0);
    {
        fancy_string_array_remove_and_destroy(a, 0);
        assert(fancy_string_array_size(a) == 0);
        fancy_string_array_remove_and_destroy(a, 100);
        assert(fancy_string_array_size(a) == 0);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_remove(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    fancy_string_array_push_value(a, "five");
    {
        fancy_string_t *s = fancy_string_array_remove(a, 3);
        assert(fancy_string_equals_value(s, "four"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_array_remove(a, 0);
        assert(fancy_string_equals_value(s, "one"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_array_remove(a, 2);
        assert(fancy_string_equals_value(s, "five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_array_remove(a, 1);
        assert(fancy_string_equals_value(s, "three"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_array_remove(a, 0);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    assert(fancy_string_array_is_empty(a));
    {
        fancy_string_t *s = fancy_string_array_remove(a, 0);
        assert(s == NULL);
        s = fancy_string_array_remove(a, 100);
        assert(s == NULL);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_get_value(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    {
        char *value = fancy_string_array_get_value(a, 1);
        fancy_string_t *s = fancy_string_create(value);
        free(value);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    {
        char *value = fancy_string_array_get_value(a, 11);
        assert(value == NULL);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_get(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_value(a, "one");
    fancy_string_array_push_value(a, "two");
    fancy_string_array_push_value(a, "three");
    fancy_string_array_push_value(a, "four");
    {
        fancy_string_t *s = fancy_string_array_get(a, 1);
        assert(fancy_string_equals_value(s, "two"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_array_get(a, 11);
        assert(s == NULL);
    }
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_push(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    assert(fancy_string_array_size(a) == 0);
    fancy_string_t *s = fancy_string_create("hello");
    fancy_string_array_push(a, s);
    assert(fancy_string_array_size(a) == 1);
    fancy_string_array_push(a, s);
    assert(fancy_string_array_size(a) == 2);
    fancy_string_array_push(a, s);
    assert(fancy_string_array_size(a) == 3);
    fancy_string_t *s_joined = fancy_string_array_join_by_value(a, " | ");
    assert(fancy_string_equals_value(s_joined, "hello | hello | hello"));
    fancy_string_destroy(s_joined);
    fancy_string_destroy(s);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_push_values(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_push_values(a, "one", "two", "three", "four", NULL);
    assert(fancy_string_array_size(a) == 4);
    fancy_string_array_push_values(a, "five", "six", "seven", "eight", "nine", NULL);
    assert(fancy_string_array_size(a) == 9);
    fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
    assert(fancy_string_equals_value(s, "one two three four five six seven eight nine"));
    fancy_string_array_push_values(a, "ten", NULL, "ignored", NULL);
    assert(fancy_string_array_size(a) == 10);
    fancy_string_array_push_values(a, NULL, "ignored", "also ignored", NULL);
    assert(fancy_string_array_size(a) == 10);
    fancy_string_destroy(s);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_push_value(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_push_value(a, "Hello");
    fancy_string_array_push_value(a, ", ");
    fancy_string_array_push_value(a, "World");
    fancy_string_array_push_value(a, "!");
    assert(fancy_string_array_size(a) == 4);
    fancy_string_t *s = fancy_string_array_join_by_value(a, "");
    assert(fancy_string_equals_value(s, "Hello, World!"));
    fancy_string_destroy(s);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_is_empty(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    assert(fancy_string_array_size(a) == 0);
    assert(fancy_string_array_is_empty(a));
    fancy_string_array_push_value(a, "no longer empty");
    assert(fancy_string_array_size(a) == 1);
    assert(!fancy_string_array_is_empty(a));
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_size(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_push_value(a, "hello");
    assert(fancy_string_array_size(a) == 1);
    fancy_string_array_push_value(a, "world");
    assert(fancy_string_array_size(a) == 2);
    fancy_string_array_clear(a);
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_clear(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("hello world");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    assert(fancy_string_array_size(a) == 2);
    fancy_string_array_clear(a);
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_print(void)
{
    LOG();
    {
        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "Hello");
        fancy_string_array_push_value(a, ", ");
        fancy_string_array_push_value(a, "World");
        fancy_string_array_push_value(a, "!");
        if (!PRINT_TO_STDOUT_DISABLED)
        {
            fancy_string_array_print(a, stdout, true);
        }
        fancy_string_array_destroy(a);
    }
    {
        char *buffer;
        size_t buffer_size;

        // [open_memestream](https://man7.org/linux/man-pages/man3/open_memstream.3.html)

        FILE *fake_stdout = open_memstream(&buffer, &buffer_size);

        fancy_string_array_t *a = fancy_string_array_create();
        fancy_string_array_push_value(a, "Hello");
        fancy_string_array_push_value(a, ", ");
        fancy_string_array_push_value(a, "World");
        fancy_string_array_push_value(a, "!");
        fancy_string_array_print(a, fake_stdout, false);

        // From the docs: "... these pointers are updated each time the stream is flushed and when the stream is closed..."
        // So it's important to call this before using `buffer`
        fclose(fake_stdout);

        fancy_string_t *s_1 = fancy_string_array_join_by_value(a, "");
        fancy_string_t *s_2 = fancy_string_create(buffer);
        assert(fancy_string_equals(s_1, s_2));

        fancy_string_destroy(s_1);
        fancy_string_destroy(s_2);

        fancy_string_array_destroy(a);

        free(buffer);
    }
}

void test_fancy_string_array_destroy(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    fancy_string_array_destroy(a);
}

void test_fancy_string_array_append_and_destroy(void)
{
    LOG();
    fancy_string_array_t *a_1 = fancy_string_array_create();
    fancy_string_array_push_value(a_1, "v1");
    fancy_string_array_t *a_2 = fancy_string_array_create();
    fancy_string_array_push_value(a_2, "v2");
    fancy_string_array_push_value(a_2, "v3");
    fancy_string_array_append_and_destroy(a_1, a_2);
    assert(fancy_string_array_size(a_1) == 3);
    fancy_string_t *s = fancy_string_array_join_by_value(a_1, " ");
    assert(fancy_string_equals_value(s, "v1 v2 v3"));
    fancy_string_destroy(s);
    // a_2 already destroyed from preview "append" call
    fancy_string_array_destroy(a_1);
}

void test_fancy_string_array_append(void)
{
    LOG();
    fancy_string_array_t *a_1 = fancy_string_array_create();
    fancy_string_array_push_value(a_1, "v1");
    fancy_string_array_t *a_2 = fancy_string_array_create();
    fancy_string_array_push_value(a_2, "v2");
    fancy_string_array_push_value(a_2, "v3");
    fancy_string_array_append(a_1, a_2);
    assert(fancy_string_array_size(a_1) == 3);
    fancy_string_t *s = fancy_string_array_join_by_value(a_1, " ");
    assert(fancy_string_equals_value(s, "v1 v2 v3"));
    fancy_string_destroy(s);
    fancy_string_array_destroy(a_2);
    fancy_string_array_destroy(a_1);
}

void test_fancy_string_array_clone(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("hello world !");
    fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
    fancy_string_array_t *clone = fancy_string_array_clone(a);
    size_t n = fancy_string_array_size(a);
    assert(n == fancy_string_array_size(clone));
    for (size_t i = 0; i < n; i++)
    {
        fancy_string_t *s_1 = fancy_string_array_get(a, i);
        fancy_string_t *s_2 = fancy_string_array_get(clone, i);
        assert(fancy_string_equals(s_1, s_2));
        fancy_string_destroy(s_1);
        fancy_string_destroy(s_2);
    }
    fancy_string_array_destroy(clone);
    fancy_string_array_destroy(a);
    fancy_string_destroy(s);
}

void test_fancy_string_array_create_with_values(void)
{
    LOG();
    {
        fancy_string_array_t *a = fancy_string_array_create_with_values(
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine",
            NULL);
        assert(fancy_string_array_size(a) == 9);
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "one two three four five six seven eight nine"));
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_array_t *a = fancy_string_array_create_with_values(
            "one",
            "two",
            NULL, // the rest will therefore be ignored
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine",
            NULL);
        assert(fancy_string_array_size(a) == 2);
        fancy_string_t *s = fancy_string_array_join_by_value(a, " ");
        assert(fancy_string_equals_value(s, "one two"));
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_array_t *a = fancy_string_array_create_with_values(
            NULL,
            "will be ignored",
            "will be ignored too",
            NULL);
        assert(fancy_string_array_size(a) == 0);
        fancy_string_array_destroy(a);
    }
}

void test_fancy_string_array_create(void)
{
    LOG();
    fancy_string_array_t *a = fancy_string_array_create();
    assert(fancy_string_array_size(a) == 0);
    fancy_string_array_destroy(a);
}

void test_fancy_string_uppercased(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("One Two Three Four FIVE");
    fancy_string_t *s_uppercased = fancy_string_uppercased(s);
    assert(fancy_string_equals_value(s, "One Two Three Four FIVE"));
    assert(fancy_string_equals_value(s_uppercased, "ONE TWO THREE FOUR FIVE"));
    fancy_string_destroy(s_uppercased);
    fancy_string_destroy(s);
}

void test_fancy_string_uppercase(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("One Two Three Four FIVE");
    fancy_string_uppercase(s);
    assert(fancy_string_equals_value(s, "ONE TWO THREE FOUR FIVE"));
    fancy_string_destroy(s);
}

void test_fancy_string_lowercased(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("One Two Three Four FIVE");
    fancy_string_t *s_lowercased = fancy_string_lowercased(s);
    assert(fancy_string_equals_value(s, "One Two Three Four FIVE"));
    assert(fancy_string_equals_value(s_lowercased, "one two three four five"));
    fancy_string_destroy(s_lowercased);
    fancy_string_destroy(s);
}

void test_fancy_string_lowercase(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("One Two Three Four FIVE");
    fancy_string_lowercase(s);
    assert(fancy_string_equals_value(s, "one two three four five"));
    fancy_string_destroy(s);
}

void test_fancy_string_replaced(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("one two three four");
        fancy_string_t *s_old = fancy_string_create("t");
        fancy_string_t *s_new = fancy_string_create("[T]");
        fancy_string_t *s_replaced = fancy_string_replaced(s, s_old, s_new, -1);
        assert(fancy_string_equals_value(s_replaced, "one [T]wo [T]hree four"));
        fancy_string_destroy(s_replaced);
        fancy_string_destroy(s_new);
        fancy_string_destroy(s_old);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_replaced_value(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_t *s_replaced = fancy_string_replaced_value(s, "o", "[o]", -1);
        assert(fancy_string_equals_value(s_replaced, "[o]ne tw[o] three f[o]ur five"));
        fancy_string_destroy(s_replaced);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_replace(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("one two three four");
        fancy_string_t *s_old = fancy_string_create("t");
        fancy_string_t *s_new = fancy_string_create("[T]");
        fancy_string_replace(s, s_old, s_new, -1);
        assert(fancy_string_equals_value(s, "one [T]wo [T]hree four"));
        fancy_string_destroy(s_new);
        fancy_string_destroy(s_old);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four");
        fancy_string_t *s_old = fancy_string_create("NOT_FOUND");
        fancy_string_t *s_new = fancy_string_create("");
        fancy_string_replace(s, s_old, s_new, -1);
        assert(fancy_string_equals_value(s, "one two three four"));
        fancy_string_destroy(s_new);
        fancy_string_destroy(s_old);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four");
        fancy_string_t *s_old = fancy_string_create("two three");
        fancy_string_t *s_new = fancy_string_create("2-3");
        fancy_string_replace(s, s_old, s_new, -1);
        assert(fancy_string_equals_value(s, "one 2-3 four"));
        fancy_string_destroy(s_new);
        fancy_string_destroy(s_old);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_replace_value(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "o", "[o]", -1);
        assert(fancy_string_equals_value(s, "[o]ne tw[o] three f[o]ur five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "o", "[o]", 0);
        assert(fancy_string_equals_value(s, "one two three four five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "o", "[o]", 1);
        assert(fancy_string_equals_value(s, "[o]ne two three four five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "o", "[o]", 2);
        assert(fancy_string_equals_value(s, "[o]ne tw[o] three four five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "o", "[o]", 3);
        assert(fancy_string_equals_value(s, "[o]ne tw[o] three f[o]ur five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "o", "[o]", 4);
        assert(fancy_string_equals_value(s, "[o]ne tw[o] three f[o]ur five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "A", "[A]", -1);
        assert(fancy_string_equals_value(s, "one two three four five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, " ", "_-_", -1);
        assert(fancy_string_equals_value(s, "one_-_two_-_three_-_four_-_five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, " ", "_-_", 2);
        assert(fancy_string_equals_value(s, "one_-_two_-_three four five"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five");
        fancy_string_replace_value(s, "e", "3", -1);
        assert(fancy_string_equals_value(s, "on3 two thr33 four fiv3"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("wordwordword");
        fancy_string_replace_value(s, "word", "w", -1);
        assert(fancy_string_equals_value(s, "www"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("wordwordword");
        fancy_string_replace_value(s, "word", "w", 1);
        assert(fancy_string_equals_value(s, "wwordword"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("wordwordword");
        fancy_string_replace_value(s, "word", "w", 2);
        assert(fancy_string_equals_value(s, "wwword"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("wordwordword");
        fancy_string_replace_value(s, "word", "", 2);
        assert(fancy_string_equals_value(s, "word"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("");
        fancy_string_replace_value(s, "word", "WORD", -1);
        assert(fancy_string_equals_value(s, ""));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("w w w");
        fancy_string_replace_value(s, "w", "WORD", -1);
        assert(fancy_string_equals_value(s, "WORD WORD WORD"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("w w w");
        fancy_string_replace_value(s, "w", "WORD", 2);
        assert(fancy_string_equals_value(s, "WORD WORD w"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("w w w");
        fancy_string_replace_value(s, "w", "WORD", 1);
        assert(fancy_string_equals_value(s, "WORD w w"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("w w w w");
        fancy_string_replace_value(s, "w w w", "w-w-w", -1);
        assert(fancy_string_equals_value(s, "w-w-w w"));
        fancy_string_destroy(s);
    }
}

void test_fancy_string_padded_end(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("222");
    {
        fancy_string_t *padded = fancy_string_padded_end(s, 5, '1');
        assert(fancy_string_equals_value(padded, "22211"));
        fancy_string_destroy(padded);
    }
    {
        fancy_string_t *padded = fancy_string_padded_end(s, 0, '1');
        assert(fancy_string_equals_value(padded, "222"));
        fancy_string_destroy(padded);
    }
    {
        fancy_string_t *padded = fancy_string_padded_end(s, 8, '2');
        assert(fancy_string_equals_value(padded, "22222222"));
        fancy_string_destroy(padded);
    }
    fancy_string_destroy(s);
}

void test_fancy_string_pad_end(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("11");
    fancy_string_pad_end(s, 4, '0');
    assert(fancy_string_equals_value(s, "1100"));
    fancy_string_update_value(s, "11");
    fancy_string_pad_end(s, 2, '0');
    assert(fancy_string_equals_value(s, "11"));
    fancy_string_clear(s);
    fancy_string_pad_end(s, 2, '0');
    assert(fancy_string_equals_value(s, "00"));
    fancy_string_destroy(s);
}

void test_fancy_string_padded_start(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("222");
    {
        fancy_string_t *padded = fancy_string_padded_start(s, 5, '1');
        assert(fancy_string_equals_value(padded, "11222"));
        fancy_string_destroy(padded);
    }
    {
        fancy_string_t *padded = fancy_string_padded_start(s, 0, '1');
        assert(fancy_string_equals_value(padded, "222"));
        fancy_string_destroy(padded);
    }
    {
        fancy_string_t *padded = fancy_string_padded_start(s, 8, '2');
        assert(fancy_string_equals_value(padded, "22222222"));
        fancy_string_destroy(padded);
    }
    fancy_string_destroy(s);
}

void test_fancy_string_pad_start(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("11");
    fancy_string_pad_start(s, 4, '0');
    assert(fancy_string_equals_value(s, "0011"));
    fancy_string_update_value(s, "111");
    fancy_string_pad_start(s, 4, '0');
    assert(fancy_string_equals_value(s, "0111"));
    fancy_string_update_value(s, "1111");
    fancy_string_pad_start(s, 4, '0');
    assert(fancy_string_equals_value(s, "1111"));
    fancy_string_clear(s);
    fancy_string_pad_start(s, 4, '0');
    assert(fancy_string_equals_value(s, "0000"));
    fancy_string_update_value(s, "12345");
    fancy_string_pad_start(s, 4, '0');
    assert(fancy_string_equals_value(s, "12345"));
    fancy_string_destroy(s);
}

void test_fancy_string_split(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("one two three four five size seven");
        fancy_string_t *sep = fancy_string_create(" ");
        fancy_string_array_t *a = fancy_string_split(s, sep, -1);
        assert(fancy_string_array_size(a) == 7);
        fancy_string_destroy(sep);
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five size seven");
        fancy_string_t *sep = fancy_string_create_empty();
        fancy_string_array_t *a = fancy_string_split(s, sep, -1);
        assert(fancy_string_array_size(a) == 1);
        fancy_string_destroy(sep);
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five size seven");
        fancy_string_t *sep = fancy_string_create("one two three four five size seven");
        fancy_string_array_t *a = fancy_string_split(s, sep, -1);
        assert(fancy_string_array_size(a) == 2);
        fancy_string_destroy(sep);
        fancy_string_destroy(s);
        fancy_string_array_destroy(a);
    }
}

void test_fancy_string_split_by_value(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("one two three four five six seven eight nine ten");
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
        assert(fancy_string_array_size(a) == 10);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, "", -1);
        assert(fancy_string_array_size(a) == 1);
        {
            fancy_string_t *s_0 = fancy_string_array_get(a, 0);
            assert(fancy_string_equals_value(s_0, value));
            fancy_string_destroy(s_0);
        }
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create("one two three four five six seven eight nine ten");
        fancy_string_array_t *a = fancy_string_split_by_value(s, "one", -1);
        assert(fancy_string_array_size(a) == 2);
        {
            fancy_string_t *s_0 = fancy_string_array_get(a, 0);
            assert(fancy_string_size(s_0) == 0);
            fancy_string_destroy(s_0);
        }
        {
            fancy_string_t *s_1 = fancy_string_array_get(a, 1);
            assert(fancy_string_size(s_1) == 45);
            fancy_string_destroy(s_1);
        }
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, value, -1);
        assert(fancy_string_array_size(a) == 2);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, "t", -1);
        assert(fancy_string_array_size(a) == 5);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, "ten", -1);
        assert(fancy_string_array_size(a) == 2);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    // testing the n_max_splits
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", -1);
        assert(fancy_string_array_size(a) == 10);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", 10);
        assert(fancy_string_array_size(a) == 10);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", 9);
        assert(fancy_string_array_size(a) == 10);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", 8);
        assert(fancy_string_array_size(a) == 9);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", 7);
        assert(fancy_string_array_size(a) == 8);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
    {
        char *value = "one two three four five six seven eight nine ten";
        fancy_string_t *s = fancy_string_create(value);
        fancy_string_array_t *a = fancy_string_split_by_value(s, " ", 1);
        {
            fancy_string_t *s_1 = fancy_string_array_get(a, 0);
            assert(fancy_string_equals_value(s_1, "one"));
            fancy_string_destroy(s_1);

            fancy_string_t *s_2 = fancy_string_array_get(a, 1);
            assert(fancy_string_equals_value(s_2, "two three four five six seven eight nine ten"));
            fancy_string_destroy(s_2);
        }
        assert(fancy_string_array_size(a) == 2);
        fancy_string_array_destroy(a);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_trimmed(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create(" this is a test ");
    {
        fancy_string_t *trimmed = fancy_string_trimmed(s);
        assert(fancy_string_equals_value(trimmed, "this is a test"));
        fancy_string_destroy(trimmed);
    }
    fancy_string_update_value(s, " \n\t\v\f\r hey\n\t\v\f\r ");
    {
        fancy_string_t *trimmed = fancy_string_trimmed(s);
        assert(fancy_string_equals_value(trimmed, "hey"));
        fancy_string_destroy(trimmed);
    }
    fancy_string_update_value(s, "hello");
    {
        fancy_string_t *trimmed = fancy_string_trimmed(s);
        assert(fancy_string_equals_value(trimmed, "hello"));
        fancy_string_destroy(trimmed);
    }
    fancy_string_destroy(s);
}

void test_fancy_string_trim(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create(" \n\t\v\f\r hey\n\t\v\f\r ");
    fancy_string_trim(s);
    assert(fancy_string_equals_value(s, "hey"));
    fancy_string_trim(s);
    assert(fancy_string_equals_value(s, "hey"));
    fancy_string_clear(s);
    fancy_string_trim(s);
    assert(fancy_string_equals_value(s, ""));
    fancy_string_update_value(s, ". this is a test .");
    fancy_string_trim(s);
    assert(fancy_string_equals_value(s, ". this is a test ."));
    fancy_string_destroy(s);
}

void test_fancy_string_trimmed_right(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create(" this is a test ");
    {
        fancy_string_t *trimmed = fancy_string_trimmed_right(s);
        assert(fancy_string_equals_value(trimmed, " this is a test"));
        fancy_string_destroy(trimmed);
    }
    fancy_string_update_value(s, "\n\t\v\f\r hey\n\t\v\f\r ");
    {
        fancy_string_t *trimmed = fancy_string_trimmed_right(s);
        assert(fancy_string_equals_value(trimmed, "\n\t\v\f\r hey"));
        fancy_string_destroy(trimmed);
    }
    fancy_string_update_value(s, "hello");
    {
        fancy_string_t *trimmed = fancy_string_trimmed_right(s);
        assert(fancy_string_equals_value(trimmed, "hello"));
        fancy_string_destroy(trimmed);
    }
    fancy_string_destroy(s);
}

void test_fancy_string_trim_right(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("\n\t\v\f\r hey\n\t\v\f\r ");
    fancy_string_trim_right(s);
    assert(fancy_string_equals_value(s, "\n\t\v\f\r hey"));
    fancy_string_trim_right(s);
    assert(fancy_string_equals_value(s, "\n\t\v\f\r hey"));
    fancy_string_clear(s);
    fancy_string_trim_right(s);
    assert(fancy_string_equals_value(s, ""));
    fancy_string_destroy(s);
}

void test_fancy_string_trimmed_left(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create(" this is a test ");
    {
        fancy_string_t *trimmed = fancy_string_trimmed_left(s);
        assert(fancy_string_equals_value(trimmed, "this is a test "));
        fancy_string_destroy(trimmed);
    }
    fancy_string_update_value(s, "\n\t\v\f\r hey\n\t\v\f\r ");
    {
        fancy_string_t *trimmed = fancy_string_trimmed_left(s);
        assert(fancy_string_equals_value(trimmed, "hey\n\t\v\f\r "));
        fancy_string_destroy(trimmed);
    }
    fancy_string_update_value(s, "hello");
    {
        fancy_string_t *trimmed = fancy_string_trimmed_left(s);
        assert(fancy_string_equals_value(trimmed, "hello"));
        fancy_string_destroy(trimmed);
    }
    fancy_string_destroy(s);
}

void test_fancy_string_trim_left(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create(" this is a test ");
    assert(fancy_string_equals_value(s, " this is a test "));
    fancy_string_trim_left(s);
    assert(!fancy_string_equals_value(s, " this is a test "));
    assert(fancy_string_equals_value(s, "this is a test "));
    fancy_string_trim_left(s);
    assert(fancy_string_equals_value(s, "this is a test "));
    fancy_string_clear(s);
    fancy_string_trim_left(s);
    assert(fancy_string_equals_value(s, ""));
    fancy_string_update_value(s, "\n\t\v\f\r hey\n");
    fancy_string_trim_left(s);
    assert(fancy_string_equals_value(s, "hey\n"));
    fancy_string_destroy(s);
}

void test_fancy_string_substring(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test. This is only a test. This is just a test.");
    {
        fancy_string_t *ss = fancy_string_substring(s, 0, 4);
        assert(fancy_string_equals_value(ss, "This"));
        fancy_string_destroy(ss);
    }
    {
        fancy_string_t *ss = fancy_string_substring(s, 1, 1);
        assert(fancy_string_equals_value(ss, ""));
        fancy_string_destroy(ss);
    }
    {
        fancy_string_t *ss = fancy_string_substring(s, 16, 36);
        assert(fancy_string_equals_value(ss, "This is only a test."));
        fancy_string_destroy(ss);
    }
    {
        char *value = fancy_string_value(s);
        fancy_string_t *ss = fancy_string_substring(s, -1, -1);
        assert(fancy_string_equals_value(ss, value));
        free(value);
        fancy_string_destroy(ss);
    }
    {
        char *value = fancy_string_value(s);
        fancy_string_t *ss = fancy_string_substring(s, 0, strlen(value));
        assert(fancy_string_equals_value(ss, value));
        free(value);
        fancy_string_destroy(ss);
    }
    fancy_string_destroy(s);
    {
        fancy_string_t *s_2 = fancy_string_create_empty();
        {
            fancy_string_t *sub = fancy_string_substring(s_2, 0, 10);
            assert(fancy_string_is_empty(sub));
            fancy_string_destroy(sub);
        }
        fancy_string_update_value(s_2, "hello");
        {
            fancy_string_t *sub = fancy_string_substring(s_2, 5, 10);
            assert(fancy_string_is_empty(sub));
            fancy_string_destroy(sub);
        }
        {
            fancy_string_t *sub = fancy_string_substring(s_2, 10, 3);
            assert(fancy_string_size(sub) == 2);
            assert(fancy_string_equals_value(sub, "lo"));
            fancy_string_destroy(sub);
        }
        {
            fancy_string_t *sub = fancy_string_substring(s_2, 3, 10);
            assert(fancy_string_size(sub) == 2);
            assert(fancy_string_equals_value(sub, "lo"));
            fancy_string_destroy(sub);
        }
        {
            fancy_string_t *sub = fancy_string_substring(s_2, -1, 100);
            assert(fancy_string_size(sub) == fancy_string_size(s_2));
            assert(fancy_string_equals(sub, s_2));
            fancy_string_destroy(sub);
        }
        fancy_string_destroy(s_2);
    }
}

void test_fancy_string_contains(void)
{
    LOG();
    fancy_string_t *s_1 = fancy_string_create("Hello, World!");
    fancy_string_t *s_2 = fancy_string_create(", World");
    assert(fancy_string_contains(s_1, s_2));
    fancy_string_update_value(s_2, "World.");
    assert(!fancy_string_contains(s_1, s_2));
    fancy_string_update(s_2, s_1);
    assert(fancy_string_contains(s_1, s_2));
    fancy_string_clear(s_2);
    assert(!fancy_string_contains(s_1, s_2));
    fancy_string_update_value(s_2, ", ");
    assert(fancy_string_contains(s_1, s_2));
    fancy_string_destroy(s_1);
    fancy_string_destroy(s_2);
}

void test_fancy_string_contains_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test");
    assert(fancy_string_contains_value(s, "This"));
    assert(!fancy_string_contains_value(s, "Test"));
    assert(fancy_string_contains_value(s, "test"));
    assert(fancy_string_contains_value(s, "This is a test"));
    assert(fancy_string_contains_value(s, "is a"));
    fancy_string_destroy(s);
}

void test_fancy_string_index_of(void)
{
    LOG();
    fancy_string_t *haystack = fancy_string_create("This is a test");
    fancy_string_t *needle = fancy_string_create("This");
    assert(fancy_string_index_of(haystack, needle) == 0);
    fancy_string_update_value(needle, " This");
    assert(fancy_string_index_of(haystack, needle) == -1);
    fancy_string_update_value(needle, "test.");
    assert(fancy_string_index_of(haystack, needle) == -1);
    fancy_string_update_value(needle, "test");
    assert(fancy_string_index_of(haystack, needle) == 10);
    fancy_string_update(needle, haystack);
    assert(fancy_string_index_of(haystack, needle) == 0);
    fancy_string_destroy(needle);
    fancy_string_destroy(haystack);
}

void test_fancy_string_index_of_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test.");
    assert(fancy_string_index_of_value(s, "Hello") == -1);
    assert(fancy_string_index_of_value(s, " This") == -1);
    assert(fancy_string_index_of_value(s, "This") == 0);
    assert(fancy_string_index_of_value(s, "is") == 2);
    assert(fancy_string_index_of_value(s, " is") == 4);
    assert(fancy_string_index_of_value(s, " .") == -1);
    assert(fancy_string_index_of_value(s, ".") == 14);
    assert(fancy_string_index_of_value(s, "his is a test.") == 1);
    fancy_string_destroy(s);
}

void test_fancy_string_ends_with(void)
{
    LOG();
    fancy_string_t *s_1 = fancy_string_create("Hello, World!");
    fancy_string_t *s_2 = fancy_string_create(", World!");
    assert(fancy_string_ends_with(s_1, s_2));
    fancy_string_update_value(s_2, ", World.");
    assert(!fancy_string_ends_with(s_1, s_2));
    fancy_string_update_value(s_1, "Hello, World.");
    assert(fancy_string_ends_with(s_1, s_2));
    fancy_string_update(s_2, s_1);
    assert(fancy_string_ends_with(s_1, s_2));
    fancy_string_clear(s_1);
    fancy_string_clear(s_2);
    assert(fancy_string_ends_with(s_1, s_2));
    fancy_string_destroy(s_2);
    fancy_string_destroy(s_1);
}

void test_fancy_string_ends_with_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This is a test");
    assert(fancy_string_ends_with_value(s, "test"));
    fancy_string_append_value(s, ".");
    assert(!fancy_string_ends_with_value(s, "test"));
    assert(fancy_string_ends_with_value(s, "test."));
    assert(fancy_string_ends_with_value(s, "This is a test."));
    fancy_string_clear(s);
    assert(fancy_string_ends_with_value(s, ""));
    fancy_string_destroy(s);
}

void test_fancy_string_starts_with(void)
{
    LOG();
    fancy_string_t *s_1 = fancy_string_create("");
    fancy_string_t *s_2 = fancy_string_create("");
    assert(fancy_string_starts_with(s_1, s_2));
    assert(fancy_string_starts_with(s_2, s_1));
    assert(fancy_string_starts_with(s_2, s_2));
    assert(fancy_string_starts_with(s_1, s_1));
    fancy_string_update_value(s_1, "hey");
    assert(!fancy_string_starts_with(s_1, s_2));
    fancy_string_update(s_2, s_1);
    assert(fancy_string_starts_with(s_1, s_2));
    fancy_string_prepend_value(s_2, " ");
    assert(!fancy_string_starts_with(s_1, s_2));
    fancy_string_destroy(s_2);
    fancy_string_destroy(s_1);
}

void test_fancy_string_starts_with_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create_empty();
    assert(fancy_string_starts_with_value(s, ""));
    fancy_string_update_value(s, "hello there");
    assert(!fancy_string_starts_with_value(s, ""));
    assert(fancy_string_starts_with_value(s, "hello "));
    assert(fancy_string_starts_with_value(s, "hello there"));
    assert(fancy_string_starts_with_value(s, "hello th"));
    assert(!fancy_string_starts_with_value(s, "hello there "));
    assert(!fancy_string_starts_with_value(s, " hello"));

    fancy_string_destroy(s);
}

void test_fancy_string_equals(void)
{
    LOG();
    fancy_string_t *s_1 = fancy_string_create("hello");
    fancy_string_t *s_2 = fancy_string_create("hello");
    assert(fancy_string_equals(s_1, s_2));
    assert(fancy_string_equals(s_2, s_1));
    assert(fancy_string_equals(s_1, s_1));
    assert(fancy_string_equals(s_2, s_2));
    fancy_string_clear(s_2);
    assert(!fancy_string_equals(s_1, s_2));
    fancy_string_clear(s_1);
    assert(fancy_string_equals(s_1, s_2));
    fancy_string_destroy(s_2);
    fancy_string_destroy(s_1);
}

void test_fancy_string_equals_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("hello");
    assert(fancy_string_equals_value(s, "hello"));
    fancy_string_clear(s);
    assert(!fancy_string_equals_value(s, "hello"));
    assert(fancy_string_equals_value(s, ""));
    fancy_string_update_value(s, "hey");
    assert(fancy_string_equals_value(s, "hey"));
    assert(!fancy_string_equals_value(s, "hey "));
    assert(!fancy_string_equals_value(s, " hey"));
    assert(!fancy_string_equals_value(s, ""));
    assert(!fancy_string_equals_value(s, "heyyou"));
    fancy_string_destroy(s);
}

void test_fancy_string_prepend(void)
{
    LOG();
    fancy_string_t *world = fancy_string_create("World!");
    fancy_string_t *hello = fancy_string_create("Hello, ");
    fancy_string_prepend(world, hello);
    assert(fancy_string_equals_value(world, "Hello, World!"));
    fancy_string_clear(world);
    fancy_string_prepend(world, hello);
    assert(fancy_string_equals(world, hello));
    fancy_string_destroy(hello);
    fancy_string_destroy(world);
}

void test_fancy_string_prepend_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("World!");
    fancy_string_prepend_value(s, "Hello, ");
    assert(fancy_string_equals_value(s, "Hello, World!"));
    fancy_string_prepend_value(s, "");
    assert(fancy_string_equals_value(s, "Hello, World!"));
    fancy_string_clear(s);
    fancy_string_prepend_value(s, "hey");
    assert(fancy_string_equals_value(s, "hey"));
    fancy_string_destroy(s);
}

void test_fancy_string_append(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create(", World!");
    {
        fancy_string_t *s_2 = fancy_string_create("Hello");
        fancy_string_append(s_2, s);
        assert(fancy_string_equals_value(s_2, "Hello, World!"));
        fancy_string_destroy(s_2);
    }
    fancy_string_destroy(s);
}

void test_fancy_string_append_value(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("Hello");
    fancy_string_append_value(s, ", World!");
    assert(fancy_string_equals_value(s, "Hello, World!"));
    fancy_string_append_value(s, "");
    assert(fancy_string_equals_value(s, "Hello, World!"));
    fancy_string_clear(s);
    assert(fancy_string_equals_value(s, ""));
    fancy_string_append_value(s, "new value");
    assert(fancy_string_equals_value(s, "new value"));
    fancy_string_destroy(s);
}

void test_fancy_string_value(void)
{
    LOG();
    char *original_value = "hello";
    size_t n = 5;
    fancy_string_t *s = fancy_string_create(original_value);
    char *value = fancy_string_value(s);
    assert(n == strlen(value));
    assert(fancy_string_equals_value(s, value));
    assert(memcmp(original_value, value, n) == 0);
    free(value);
    fancy_string_destroy(s);
}

void test_fancy_string_update(void)
{
    LOG();
    fancy_string_t *s_1 = fancy_string_create("Hello!");
    assert(fancy_string_equals_value(s_1, "Hello!"));
    fancy_string_t *s_2 = fancy_string_create("Hey!");
    fancy_string_update(s_1, s_2);
    assert(!fancy_string_equals_value(s_1, "Hello!"));
    assert(fancy_string_equals_value(s_1, "Hey!"));
    fancy_string_destroy(s_2);
    fancy_string_destroy(s_1);
}

void test_fancy_string_update_value(void)
{
    LOG();
    char *original_value = "Original value";
    char *updated_value = "Updated value";
    fancy_string_t *s = fancy_string_create(original_value);
    assert(fancy_string_equals_value(s, original_value));
    assert(!fancy_string_equals_value(s, updated_value));
    fancy_string_update_value(s, updated_value);
    assert(!fancy_string_equals_value(s, original_value));
    assert(fancy_string_equals_value(s, updated_value));
    fancy_string_clear(s);
    assert(fancy_string_equals_value(s, ""));
    fancy_string_destroy(s);
}

void test_fancy_string_clear(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("This will get cleared up!");
    assert(!fancy_string_is_empty(s));
    fancy_string_clear(s);
    assert(fancy_string_is_empty(s));
    fancy_string_destroy(s);
}

void test_fancy_string_is_empty(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("");
    assert(fancy_string_is_empty(s));
    fancy_string_update_value(s, "not empty");
    assert(!fancy_string_is_empty(s));
    fancy_string_clear(s);
    assert(fancy_string_is_empty(s));
    fancy_string_destroy(s);
}

void test_fancy_string_size(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("Hey");
        assert(fancy_string_size(s) == 3);
        fancy_string_clear(s);
        assert(fancy_string_size(s) == 0);
        fancy_string_update_value(s, "This is a test");
        assert(fancy_string_size(s) == 14);
        fancy_string_append_value(s, ".");
        assert(fancy_string_size(s) == 15);
        fancy_string_prepend_value(s, "");
        assert(fancy_string_size(s) == 15);
        fancy_string_prepend_value(s, " This is just a test.");
        assert(fancy_string_size(s) == 36);
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create_repeat(" hey ", 2);
        assert(fancy_string_size(s) == 15);
        fancy_string_trim(s);
        assert(fancy_string_size(s) == 13);
        fancy_string_replace_value(s, "hey", "H", -1);
        assert(fancy_string_size(s) == 7);
        fancy_string_replace_value(s, "  ", " ", -1);
        assert(fancy_string_size(s) == 5);
        fancy_string_destroy(s);
    }
}

void test_fancy_string_print(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create("Hello, World!");
        if (!PRINT_TO_STDOUT_DISABLED)
        {
            fancy_string_print(s, stdout, true);
        }
        fancy_string_destroy(s);
    }
    {
        char *buffer;
        size_t buffer_size;
        // [open_memestream](https://man7.org/linux/man-pages/man3/open_memstream.3.html)
        FILE *fake_stdout = open_memstream(&buffer, &buffer_size);
        fancy_string_t *s = fancy_string_create("Hello, World!");
        fancy_string_print(s, fake_stdout, false);
        // From the docs: "... these pointers are updated each time the stream is flushed and when the stream is closed..."
        // So it's important to call this before using `buffer`
        fclose(fake_stdout);
        assert(fancy_string_equals_value(s, buffer));
        fancy_string_destroy(s);
        free(buffer);
    }

    // {
    //     FILE *file_pointer = fopen("./tmp/test/test_fancy_string_print.txt", "w");
    //     fancy_string_t *s = fancy_string_create("Hello, World!");
    //     fancy_string_print(s, file_pointer, true);
    //     fancy_string_destroy(s);
    //     fclose(file_pointer);
    // }
    // {
    //     // read this:
    //     // - https://man7.org/linux/man-pages/man3/fmemopen.3.html
    //     // - https://pubs.opengroup.org/onlinepubs/9699919799/functions/fmemopen.html

    //     printf("TESTING THE IN-MEMORY BUFFER\n");
    //     char *buffer = malloc(sizeof(char *) * (100));
    //     FILE *fp = fmemopen(buffer, 100, "w+");

    //     fancy_string_t *s = fancy_string_create("This is a test.\nNew line 1.\nNew line 2.");

    //     fancy_string_print(s, fp, false);

    //     printf("THIS IS MY BUFFER:\n%s", buffer);
    //     printf("\nsize = %zu\n", strlen(buffer));

    //     fancy_string_destroy(s);

    //     free(buffer); // not 100% sure this is need because fclose might take care of it...
    //     fclose(fp);
    //     // exit(0);
    // }
}

void test_fancy_string_destroy(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("Hello, World!");
    fancy_string_destroy(s);
}

void test_fancy_string_clone(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("Hello, World!");
    fancy_string_t *clone = fancy_string_clone(s);
    assert(fancy_string_equals(s, clone));
    fancy_string_clear(clone);
    assert(!fancy_string_equals(s, clone));
    fancy_string_destroy(clone);
    fancy_string_destroy(s);
}

void test_fancy_string_create_empty(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create_empty();
    assert(fancy_string_size(s) == 0);
    fancy_string_destroy(s);
}

void test_fancy_string_create_repeat(void)
{
    LOG();
    {
        fancy_string_t *s = fancy_string_create_repeat("abc", 2);
        assert(fancy_string_equals_value(s, "abcabcabc"));
        fancy_string_destroy(s);
    }
    {
        fancy_string_t *s = fancy_string_create_repeat("abc", 0);
        assert(fancy_string_equals_value(s, "abc"));
        fancy_string_destroy(s);
    }
}

void test_fancy_string_from_stream(void)
{
    LOG();
    // {
    //     FILE *file_pointer = fopen("./tmp/test/hello-world.txt", "r");
    //     fancy_string_t *s = fancy_string_from_stream(file_pointer);
    //     fancy_string_print(s, stdout, true);
    //     fancy_string_destroy(s);
    // }
    {
        char *value = "This is a test";
        FILE *stream = fmemopen(value, strlen(value), "r");
        fancy_string_t *s = fancy_string_from_stream(stream);
        assert(fancy_string_equals_value(s, value));
        fancy_string_destroy(s);
    }
}

void test_fancy_string_from_stream_next_line(void)
{
    LOG();
    {
        char *data = "This is a title\n\nThis is the first line.\nThis is another line.\n\nBye bye line.";
        FILE *stream = fmemopen(data, strlen(data), "r");
        fancy_string_array_t *a = fancy_string_array_create();
        while (true)
        {
            fancy_string_t *line = fancy_string_from_stream_next_line(stream);
            if (line == NULL)
            {
                break;
            }
            fancy_string_array_push(a, line);
            fancy_string_destroy(line);
        }
        assert(fancy_string_array_size(a) == 6);
        fancy_string_array_destroy(a);
        fclose(stream);
    }
    {
        FILE *file_pointer = fopen("./test/assets/multi-line-file.txt", "r");
        fancy_string_array_t *a = fancy_string_array_create();
        while (true)
        {
            fancy_string_t *line = fancy_string_from_stream_next_line(file_pointer);
            if (line == NULL)
            {
                break;
            }
            fancy_string_array_push(a, line);
            fancy_string_destroy(line);
        }
        assert(fancy_string_array_size(a) == 12);
        {
            fancy_string_t *should_be_null = fancy_string_from_stream_next_line(NULL);
            assert(should_be_null == NULL);
        }
        fclose(file_pointer);
        {
            fancy_string_t *should_be_null = fancy_string_from_stream_next_line(NULL);
            assert(should_be_null == NULL);
        }
        fancy_string_array_destroy(a);
    }
    {
        fancy_string_t *line = fancy_string_from_stream_next_line(NULL);
        assert(line == NULL);
    }
}

void test_fancy_string_create(void)
{
    LOG();
    fancy_string_t *s = fancy_string_create("Hello, World!");
    char *value = fancy_string_value(s);
    assert(fancy_string_equals_value(s, value));
    free(value);
    fancy_string_destroy(s);
}

void test_fancy_string_library_version_print(void)
{
    LOG();
    if (!PRINT_TO_STDOUT_DISABLED)
    {
        fancy_string_library_version_print(stdout);
    }
}

void test_fancy_string_library_version(void)
{
    // LOG();
    uint16_t major;
    uint16_t minor;
    uint16_t revision;
    fancy_string_library_version(&major, &minor, &revision);
    if (!PRINT_TO_STDOUT_DISABLED)
    {
        fprintf(stdout, "\033[34mRunning unit tests for \033[33mlibfancy_string.%i.%i.%i\033[0m\n", major, minor, revision);
    }
}
