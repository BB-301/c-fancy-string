#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

#include "fancy_string.h"

static void *thread_callback(void *context);

typedef enum my_thread_type_e
{
    MY_THREAD_TYPE_WATCHER,
    MY_THREAD_TYPE_WRITER,
} my_thread_type_t;

typedef struct my_context_s
{
    my_thread_type_t thread_type;
} my_context_t;

int main(void)
{
    // NOTE: Use `FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL` instead of
    // `FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC` to see what happens with
    // "thread local" memory tracking mode.
    fancy_string_memory_usage_init(FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC);

    pthread_t thread_1;
    my_context_t thread_1_context = {.thread_type = MY_THREAD_TYPE_WRITER};
    if (pthread_create(&thread_1, NULL, &thread_callback, &thread_1_context) != 0)
    {
        perror("Failed to create thread #1 (i.e., writer thread)");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_2;
    my_context_t thread_2_context = {.thread_type = MY_THREAD_TYPE_WATCHER};
    if (pthread_create(&thread_2, NULL, &thread_callback, &thread_2_context) != 0)
    {
        perror("Failed to create thread #2 (i.e., watcher thread)");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(thread_1, NULL) != 0)
    {
        perror("Failed to join thread #1 (i.e., writer thread)");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(thread_2, NULL) != 0)
    {
        perror("Failed to join thread #2 (i.e., watcher thread)");
        exit(EXIT_FAILURE);
    }
}

static void *thread_callback(void *context)
{
    my_context_t *c = (my_context_t *)context;

    size_t iteration_counter = 0;

    if (c->thread_type == MY_THREAD_TYPE_WATCHER)
    {
        bool started = false;
        size_t usage = 0;

        while (!started || usage > 0)
        {
            iteration_counter += 1;

            if (iteration_counter >= 20)
            {
                break;
            }

            usage = fancy_string_memory_usage_get();
            if (!started && usage != 0)
            {
                started = true;
            }

            fprintf(stdout, "[watcher:%zu]\ttotal memory usage: %zu bytes\n", iteration_counter, usage);

            sleep(1);
        }

        fprintf(stdout, "[watcher:%zu]\texiting thread\n", iteration_counter);
    }
    else if (c->thread_type == MY_THREAD_TYPE_WRITER)
    {
        fancy_string_t *s = fancy_string_create("hello");

        while (true)
        {
            iteration_counter += 1;

            size_t usage = fancy_string_memory_usage_get();
            fprintf(stdout, "[writer:%zu]\ttotal memory usage: %zu bytes\n", iteration_counter, usage);
            if (usage > 300)
            {
                break;
            }
            sleep(2);
            fancy_string_append_value(s, "");
            fancy_string_append(s, s);
        }

        fancy_string_destroy(s);

        fprintf(stdout, "[writer:%zu]\texiting thread\n", iteration_counter);
    }
    else
    {
        fprintf(stderr, "This is not possible!\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}
