#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

#define USE_VERBOSE_DEBUGGING 1

static size_t get_and_print_current_usage(void);

int main(void)
{
    fancy_string_memory_usage_init(FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL);

    size_t current_usage;

    current_usage = get_and_print_current_usage();
    assert(current_usage == 0);

    fancy_string_t *s = fancy_string_create("This will use memory");

    current_usage = get_and_print_current_usage();
    assert(current_usage > 0);

    fancy_string_update_value(s, "less memory");
    size_t new_usage = get_and_print_current_usage();
    assert(new_usage > 0);
    assert(current_usage > new_usage);

    current_usage = new_usage;

    fancy_string_destroy(s);

    current_usage = get_and_print_current_usage();
    assert(current_usage == 0);

    exit(EXIT_SUCCESS);
}

static size_t get_and_print_current_usage(void)
{
    size_t current_usage = fancy_string_memory_usage_get();
    fprintf(stdout, "----\nCurrent usage: %zu bytes\n", current_usage);
#if (USE_VERBOSE_DEBUGGING == 1)
    fancy_string_memory_usage_debug(stdout);
#endif
    return current_usage;
}
