#include <stdio.h>
#include <assert.h>

#include "fancy_string.h"

int main(void)
{
    fancy_string_t *s = fancy_string_create("1");
    fancy_string_pad_start(s, 4, '0');
    assert(fancy_string_equals_value(s, "0001"));
    fancy_string_print(s, stdout, true);

    return 0;
}
