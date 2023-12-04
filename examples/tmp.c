#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

struct my_fancy_string_s
{
    char *value;
    size_t n;
};

static void demo(void);

int main(void)
{
    fancy_string_t *s = fancy_string_create("This is the object's initial internal state.");

    fancy_string_prepend_value(s, "PREFIX: ");
    assert(fancy_string_equals_value(s, "PREFIX: This is the object's initial internal state."));
    fancy_string_print(s, stdout, true);

    struct my_fancy_string_s *hacked_string = (struct my_fancy_string_s *)((void *)s);
    fprintf(stdout, "Hacked value (size = %zu): %s\n", hacked_string->n, hacked_string->value);

    fancy_string_destroy(s);

    demo();

    return 0;
}

static void demo(void)
{
    fancy_string_t *s = fancy_string_create("I'm not a hacker!");
    char *s_value = fancy_string_value(s);
    size_t s_size = fancy_string_size(s);
    fprintf(stdout, "Internal state copy (size = %zu): %s\n", s_size, s_value);
    free(s_value);
    fancy_string_destroy(s);
}
