#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fancy_string.h"

int main(void)
{
    // This first example illustrates how to use `fancy_string_from_stream_next_line`
    // to read a fake stream (generated using "fmemopen") line by line.
    {
        char *data = "This is a title\n\nThis is the first line.\nThis is another line.\n\nBye bye line.";
        FILE *stream = fmemopen(data, strlen(data), "r");
        while (true)
        {
            fancy_string_t *line = fancy_string_from_stream_next_line(stream);
            if (line == NULL)
            {
                break;
            }
            fancy_string_print(line, stdout, true);
            fancy_string_destroy(line);
        }

        fclose(stream);
    }

    // This second example illustrates how to use `fancy_string_from_stream_next_line`
    // to read a file stream line by line.
    {
        FILE *file_pointer = fopen("./test/assets/multi-line-file.txt", "r");

        while (true)
        {
            fancy_string_t *line = fancy_string_from_stream_next_line(file_pointer);
            if (line == NULL)
            {
                break;
            }
            fancy_string_print(line, stdout, true);
            fancy_string_destroy(line);
        }

        fclose(file_pointer);
    }

    // This third example illustrates how to use `fancy_string_from_stream_next_line`
    // to read from `stdin`. Each new line will be echoed back, and the loop will
    // break if the "exit" command is entered.
    // NOTE: The example is disabled by default: to use it, change `false` to `true`.
    if (false)
    {
        printf("\n\nPlease write something and press enter. You can write 'exit' to terminate.\n");
        while (true)
        {
            fancy_string_t *line = fancy_string_from_stream_next_line(stdin);

            if (fancy_string_equals_value(line, "exit"))
            {
                fancy_string_destroy(line);
                break;
            }

            fancy_string_print(line, stdout, true);
            fancy_string_destroy(line);
        }
    }

    return 0;
}
