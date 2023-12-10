# lib<fancy_string>

**A C library for easy and fun string manipulation.**

## Introduction

I have loved the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) ever since I started writing computer programs. But, unfortunately, I feel like I don't always get the chance to use it as much as I'd want to. Oftentimes, I have a small task to automate, but my first reflex is never to start writing a simple C program: I always pick something else, like [TypeScript](https://www.typescriptlang.org/) and [NodeJS](https://nodejs.org/en) or [Rust](https://www.rust-lang.org/). I guess my main blockage for using C as a scripting language is the fact that I'm struggling with quick and efficient string manipulation in that language. It's kind of a vicious cycle: since I never use C for scripting, I never get better and faster at C strings, so, when the time comes to write a quick program, I don't use C, because I don't feel it would be quick enough...

The other day (i.e., in the last quarter of 2023), I was playing around with the [Mosquitto library](https://github.com/eclipse/mosquitto), trying to build a simple [MQTT](https://en.wikipedia.org/wiki/MQTT) client process, and was faced with the same bottleneck again: I wanted to perform some very basic topic-based routing, but I ended up wasting (investing?) more time than I would have wanted to doing string manipulation using the functions defined in `string.h`. But, as my toy program started shaping up, having created nice helper functions for some of the string operations I needed, I got the idea of turning some of those functions into their own library, with an API similar to [JavaScript's String API](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String). I didn't stop there: while at it, I also extended my library to mimic [JavaScript's Array API](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array). And that's how this project was born!

I named it <em>`lib<fancy_string>`, a C library for easy and fun string manipulation.</em>

### But why the name `fancy_string`?

As I'm also explaining in my [c-fancy-memory](https://github.com/BB-301/c-fancy-memory/tree/main#why-the-name-fancy_memory) repository, the main reason for the name is that I always try to namespace my C functions with prefixes that have very low probability of conflicting with function names from other libraries, so, in this case, I am pretty confident that not a lot of programmers will use the term `fancy_{something}` as prefix for their custom C types. Moreover, when working with most decent text editor or IDE, I find it much simpler to find a specific library function by simply starting to write the prefix (and then the full list of functions and types pops up).

Another reason is that, to me, `fan` is a meaningful acronym, so I thought that it would be clever to combine that acronym with `C` (the programming language) to form `fan-C`, which sounds like `fancy` when pronouncing it.

## A quick example

```c
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
    // they take as argument a string literal. But the library also has the
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
```

## Design goals and patterns

While writing this library, my main goal was to create something that would make me faster at writing C programs involving extensive string manipulation, "script-style programs" being the perfect example. That being said, efficiency (in the sense of speed, CPU usage, and memory usage), while always nice to have, was not at the top of my list of design goals. While I did put a lot of effort to ensure that my implementation would not cause memory leaks (I've run several tests and I'm now pretty confident that it won't), the implementation of several of my library functions have not been optimized as much has they could have been. For instance, there is a lot of memory being copied around that could have been simply passed as pointers, but doing it the way I did made the implementation quicker to write, because it was less error prone to internally use some of the API functions once those functions had been tested. In other words, I bootstrapped my implementation using my own API internally.

So let's now talk about the main pattern used by the library. As visible from the [quick example](#a-quick-example) section above, this library's API uses a style some might call **C object-based programming**, in which, in order to mimic a class, an abstract data type is declared (in this case [fancy_string_t](./include/fancy_string.h#L51)) that is used as a container for storing the object's data (note that, by using an abstract data type, the "class members" are made (somewhat) private, as opposed to what would be obtained when both declaring and defining the structure inside the header file). That object is then passed as a pointer to the library's public API functions, which are referred to here as "methods". (NOTE: We can still have "object-based programming in C" without using an abstract type.)

For instance, we create the "string object" like this:
```c
fancy_string_t *s = fancy_string_create("This is the object's initial internal state.");
```
where `fancy_string_create` corresponds to the "factory method" used to instantiate an object, whose pointer gets stored in the variable named `s`.

We then use that object to perform operations like the following:
```c
fancy_string_prepend_value(s, "PREFIX: ");
assert(fancy_string_equals_value(s, "PREFIX: This is the object's initial internal state."));
```
where we add a prefix "by value" (i.e., using a string literal) to our string object pointed to by `s`, and then we check (i.e., assert) that our string object has been updated as expected. The `fancy_string_prepend_value` and `fancy_string_equals_value` functions in that previous code block are referred to here as "instance methods" (or simple "methods"), because they operate on an object (in this case, the "string object" pointed to by `s`). Note that, for all of the library's "instance methods", the first argument is always the object on which we are operating (or whose state we are interrogating).

Just for fun, the following block illustrates how to use the `fancy_string_print` method to write a summary of the string object pointed to by `s` to `stdout`:
```c
fancy_string_print(s, stdout, true);
```
where the third argument is a boolean value indicating whether to print in "debug/verbose" mode.

Once we are done with `s`, we need to dispose of it, else we'll leak memory. This is done as follows:
```c
fancy_string_destroy(s);
```

Now, let's go back to our example's first block of code, in which we called `fancy_string_create`, which method returned a pointer to an object of type `fancy_string_t` (which we assigned to `s`). `fancy_string_t` is an example of an abstract data type. This library contains three such types: `fancy_string_t`, `fancy_string_array_t`, and `fancy_string_regex_t`. Internally, those types are defined as structures and they are used as containers to track the objects' internal states. Let's, as an illustration, further inspect `fancy_string_t`, which, at the moment (because, who knowns, this could change in the future without affecting the public API itself), is defined as follows:
```c
struct fancy_string_s
{
    char *value;
    size_t n;
};
```

Note that `fancy_string_s` is defined inside the implementation file [fancy_string.c](./src/fancy_string.c). That's intentional, because we want the string object to be an abstract type. In the header file [fancy_string.h](./include/fancy_string.h), all we have is the following:
```c
typedef struct fancy_string_s fancy_string_t;
```

Obviously, the cultivated C programmer in the audience will quickly recognize that one could hack ones way into directly using the structure's members by doing something like this:
```c
struct my_fancy_string_s
{
    char *value;
    size_t n;
};
// ...
fancy_string_t *s = fancy_string_create("I'm a hacker!");
struct my_fancy_string_s *hacked_string = (struct my_fancy_string_s *)((void *)s);
fprintf(stdout, "Hacked value (size = %zu): %s\n", hacked_string->n, hacked_string->value);
fancy_string_destroy(s);
// ...
``` 

So that means that even though the data type is abstract, it can still be accessed directly by the application if a programmer really wants to. In general, this shouldn't be necessary, since the whole point is to have the library take care of everything for us without us having to worry about string manipulation errors (which are easy to make using lower level functions such as `malloc`, `free`, `strcpy`, `memcpy`, etc.). Nonetheless, I still wanted to bring this little hack to the reader's attention, because it might be useful under some circumstances (e.g., when reading a string value often and not wanting to get a copy of it each time).

By the way, in the previous code block, the intended way of retrieving the object's internal state would have been like this:
```c
fancy_string_t *s = fancy_string_create("I'm not a hacker!");
char* s_value = fancy_string_value(s);
size_t s_size = fancy_string_size(s);
fprintf(stdout, "Internal state copy (size = %zu): %s\n", s_size, s_value);
free(s_value);
fancy_string_destroy(s);
```
Obviously, there is a some extra overhead associated with that approach, since the library method `fancy_string_value` creates a memory-independent copy of the string data, which must then be freed by the application when no longer needed. The reason we do this is simple, however: if we return a pointer to the original data, the application could modify it, which is not something we want (at least, this is not something I want). In other words, we are making a tradeoff here: we accept the extra overhead in exchange for extra insurance; insurance that we cannot accidentally modify the object's internal state without explicitly passing it to a mutating method.

We end this section with a simple list of notable characteristics and behaviors of the library:

* As briefly mentioned in the [introduction](#introduction) section, this library's API is largely inspired from JavaScript's [String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String) and [Array](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/splice) APIs. In fact, other programming languages with which I am familiar (e.g. [Swift](https://docs.swift.org/swift-book/documentation/the-swift-programming-language/stringsandcharacters/) and [Rust](https://doc.rust-lang.org/std/string/struct.String.html)'s standard libraries) have similar APIs, which, in my opinion, make working with strings feel effortless.
* Each object that gets returned by any of the methods should be destroyed once its no longer needed (unless your program is about to exit anyways). For instance, if you call the `fancy_string_split_by_value()` method on a string object, it will return an array object. That array object, as well as the initial string object, will eventually both have to be destroyed using, `fancy_string_array_destroy()` (for the array) and `fancy_string_destroy()` (for the string). By the way, there's a third destroyer for the regular expression object; i.e., `fancy_string_regex_destroy()`. Moreover, as alluded to previously in this section (in a code block), calling `fancy_string_value` will return a copy of the string object's internal state (i.e., a string), which string, when no longer needed, will need to be freed using `free()`.
* Several of this library's instance methods have two versions of basically the same thing: one that takes extra (e.g., seconds, third, etc.) arguments as "string objects", and one that takes extra arguments as "literal strings". For instance, we have `fancy_string_append`, which expects its second argument to be a string object, while we also have `fancy_string_append_value`, which is virtually the same method as `fancy_string_append`, except that this one expects a string literal for its second argument. So, again, there are several of such method pairs, and the reason for that is that the latter type (i.e., the one suffixed with `_value`) can be very useful for using hardcoded string values while writing the program. Another example of such a pair is `fancy_string_split` and `fancy_string_split_by_value`, both of which are used to split a string object's internal state into an array object of string objects, where the separator is, for the former method, a string object itself, while, for the latter method, it corresponds to a string literal. NOTE: By string literal, here, I mean something that is declared directly in the method call, but it can also mean a variable of type `char []` passed as a pointer or a pointer of type `char *` pointing to a dynamically allocated null-terminated string.
* In several instances, the library also have pairs of methods that perform the same operation, except that one method operates on the object directly (i.e., it mutates the object's internal state), while the other method clones the object, performs the operation on that clone, and then returns it. For instance, we have `fancy_string_trim_left`, which trims a string object's internal data to the left, while we alternatively have `fancy_string_trimmed_left`, which first creates a clone of the object and then runs `fancy_string_trim_left` on that clone before returning it. In the case of `fancy_string_trimmed_left`, we will have one more object that will eventually have to be destroyed using `fancy_string_destroy`.
* Some users of this library will notice that I am using the `const` keyword unsparingly in my method declarations. For instance, if we look at the `fancy_string_print` method, we see that its signature is `void fancy_string_print(fancy_string_t const *const self, FILE *stream, bool debug)`. In that case, note the `const *const` term. Some may find the rightmost `const` to by unnecessary, and I couldn't argue with them. The only thing that the second `const` means is that it won't allow the implementation to re-assign `self` withing its scope. So, to the function user, this has no consequence at all. But, to me, it was a way of remembering, during development, that my intention was to not change to which value `self` was pointing inside the implementation, to avoid errors. That's all. But the leftmost `const` keyword is more important: it means, when present, that the value pointed to by `self` is not to be modified by the `fancy_string_print`. That said, all non-mutating methods in the library clearly identify themselves using that `fancy_string_t const...` syntax.
* The error handling mechanisms used by this library are fairly simple:
  * If a memory allocation error occurs, the library will terminate the process with a message about it printed to `stderr`.
  * Most of the library's function implementations start with asserting (using the `assert` macro) that the passed arguments are not the `NULL` pointer. Those assertions will be disabled if the library gets compiled with the `NDEBUG` preprocessor flag, however, but the process will still very likely crash.
  * Double freeing is not allowed, so, for example, calling a method like `fancy_string_destroy` more than once on the same object will likely crash the process.
  * All other errors are not fatal errors and are signaled to the application by the method returning either `NULL` or `ssize_t -1`, depending on the context. In any case, you should read the [documentation](https://bb-301.github.io/c-fancy-string-docs) for every method you use and the potential errors will be explained there.
* The library, by default, ships with a memory tracking feature that allows monitoring the memory in use by the library during development. By default, the feature gets built into the library, but, even then, the default behavior is to not track the memory, such that the overhead associated with the feature is minimal using the default configuration. More precisely, internally, the feature, in its default configuration, is simply making use of a single atomic read of an `enum` flag on each operation. However, I have yet to assess, through a simulation, what that means in terms of performance, but, for the moment, without definitive guidance, one may wish to err on the side of safety and simply choose to disable the tracking feature altogether by building the library (or the application with the library's source code as dependency) using the preprocessor flag `FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED=0` (more on this in the [next section](#start-by-cloning-the-repository)).
  * The [examples](./examples) directory contains two examples where the memory tracking feature is used:
    * [fancy_string_memory_tracking_multi_thread.c](examples/fancy_string_memory_tracking_multi_thread.c)
    * [fancy_string_memory_tracking_single_thread.c](examples/fancy_string_memory_tracking_single_thread.c)
  * The [test/main.c](./test/main.c) file also makes use of the feature to perform the integration test.
* In a few instances throughout this project's documentation and the codebase, I use the term `callback` to refer to function signatures such as, for instance, `fancy_string_find_t`, as well as functions (conforming to such signatures) being passed as arguments to methods such as, for instance, `fancy_string_array_find_index`. I'm adding a note here about this, because I don't want to cause confusion with that term. In some contexts, the term `callback` is often used to refer to a function that will be executed after an asynchronous operation completes. In other contexts, however, the term is more generally simply used to refer to a function that gets passed as argument to another function (and which is expected to be called inside that function, when needed). In the context of this library, it is the second, more general definition that applies.

## How to install

#### A note about portability

`lib<fancy_string>` is a `POSIX` C library, because it is based on types found in POSIX headers such as `unistd.h`, `regex.h`, and `pthread.h`. I may, however, in a future version, try to make the library more portable. But, for the moment, Windows users that would still like to experiment with the library could do so by using [Docker Desktop](https://www.docker.com/products/docker-desktop/) and running the examples (and their code) inside a Docker container (I recommend the official [gcc image](https://hub.docker.com/_/gcc)). Another alternative would be to slightly modify the source code to get rid of the POSIX-related features (most of which are highlighted in the [roadmap](#roadmap) section below).

### Start by cloning the repository

The first step is to clone the repository; i.e.,
```shell
git clone --recurse-submodules https://github.com/BB-301/c-fancy-string.git
```
Note the use of `--recurse-submodules`. This is because this project has an optional dependency on another project of mine called [c-fancy-memory](https://github.com/BB-301/c-fancy-memory), which, as already discussed in previous sections, is used to track memory allocations during development with this library. As also already previously mentioned, that feature can be turned off by building the library using the preprocessor flag `FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED=0`, in which case the `c-fancy-memory` dependency is not required.

### Choose an installation method

#### Install on the development system

If you want to use this library for scripting, learning, and experimenting, I recommend that you install it on your system's path. The [Makefile](./Makefile) has a recipe named `make install`, which builds and installs the binaries at `/usr/local/lib` and the header file at `/usr/local/include`. Note that the `make install` recipe uses `sudo`, so you will get a password prompt when running it.

If you also want to install the "man page", provided that Docker is running on your system, you can alternatively call `make install_with_docs`, and a simple Docker image containing [Doxygen](https://www.doxygen.nl/) will be built and run as a container to generate the documentation and install it at `/usr/local/man/man3/fancy_string.h.3` (in addition to installing the library binaries and the header file) . Then, you'll be able to use `man 3 fancy_string.h` in a terminal to open the documentation.

#### Use as a dependency inside a C project

If you are developing an application (or a library) and want to use `lib<fancy_string>` as a dependency, I recommend that you use a [Git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) (provided that you are using Git for your project). For instance, from your project's root directory, you could run something like this:
```shell
git submodule add https://github.com/BB-301/c-fancy-string.git
```

The other alternative, if you are not using Git, or if you don't want to use a Git submodule, is to simply copy [fancy_string.h](./include/fancy_string.h) and [fancy_string.c](./src/fancy_string.c) and to manually add them to your project, along with the library's [LICENSE](./LICENSE) information.

## Documentation

The library's API is documented using the [Doxygen](https://www.doxygen.nl/index.html) format (see [fancy_string.h](./include/fancy_string.h)), such that it (the library) will play nicely with IDEs supporting that code documentation format, but, for convenience, this repository also has its own [GitHub-hosted Doxygen website](https://bb-301.github.io/c-fancy-string-docs), which should be consulted for a good overview of the library's API.

If you're interested, you may also clone this repository and generate the website yourself using the `make docs` recipe, which, however, assumes that you have [Docker](https://www.docker.com/) (or Docker Desktop) installed and running on your system. If you already have Doxygen installed on your system, you may modify the [Makefile](./Makefile) to use it instead of Docker, but please note that I used Doxygen version `1.9.6` while developing, so if you're using another version, it's possible that you don't get the intended result.

Finally, you can also have a look at the [examples](./examples) directory for quick examples about how this library can be used. Note that most of those examples are also embedded directly into the API documentation.

## Roadmap

* I want to carefully review all of the text (here, and in the code documentation) for typos and grammatical errors. I know that there probably still are plenty of them left, but I didn't want to delay the release of this first version of the library any longer, because I wanted to start testing it inside real applications, so I made a compromise. So just know, dear reader/user, that I am aware (at least, I suspect) that there still are typos and grammatical errors, but that I will eventually catch them and fix them.
* I want to create an exhaustive series of tutorials on how to use the various features exposed in the library's API.
* I want to create demo projects based on the library, and share those projects here, in addition to the tutorial series.
* I want to assess (for example, through simple simulation studies) what kind of penalties should be expected when using this library, as compared to performing "vanilla" string manipulation (i.e., using only the facilities declared in the C Standard Library's `string.h` and `stdlib.h` header files).
* As mentioned in the [installation section](#a-note-about-portability), the library is currently not fully compatible with Windows systems. The main reason for this is that, internally, I am making use of facilities such as POSIX [regular expressions](https://en.wikibooks.org/wiki/Regular_Expressions/POSIX-Extended_Regular_Expressions) and [pthreads](https://man7.org/linux/man-pages/man7/pthreads.7.html) (the latter is only used when the memory tracking feature is enabled, however). I am also using, in the [test/main.c](./test/main.c), functions such as [fmemopen](https://man7.org/linux/man-pages/man3/fmemopen.3.html), which latter I don't think is available on Windows either. To be fair, I don't think that porting this library to Windows would require that much work, so it is something that I plan on doing in a future release. In the meantime, Windows users that would like to use the library could, as alluded to earlier, simply choose to slightly modify the source code where needed, or develop and run their application inside a Docker container (I recommend the official [gcc image](https://hub.docker.com/_/gcc)). 
* I may, in the future, rework the implementation for some of the library's methods, in order to make them more efficient.

## Files and directories explained

* c-fancy-memory — An optional directory, which corresponds to a Git submodule, and which is used as a dependency by this library to implement its memory monitoring feature. If you don't want to fetch that dependency, please make sure to compile the library using the preprocessor flag `FANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED=0`.
* [doxygen](./doxygen) — A directory that contains Doxygen-related stuff used to generate the [API documentation website](https://bb-301.github.io/c-fancy-string-docs) for this library.
* [examples](./examples) — A directory that contains standalone examples illustrating how the library's different methods can be used. Those examples are also contained in the [API documentation website](https://bb-301.github.io/c-fancy-string-docs), to give additional details on how certain methods work. The [Makefile](./Makefile) declares a recipe for each example. For instance, to run [examples/a_quick_example.c](examples/a_quick_example.c) simply run `make a_quick_example` (without the `.c` extension at the end of the file name). For a list of all example-related recipes, run `make examples`.
* [include](./include) — A directory that contains the header file [fancy_string.h](./include/fancy_string.h); i.e., the declarations for the library's public API.
* [src](./src) — A directory that contains the implementation file [fancy_string.c](./src/fancy_string.c), in which all of the definitions for the methods declared in [fancy_string.h](./include/fancy_string.h) are provided.
* [test](./test) — A directory that contains a file (i.e., [main.c](./test/main.c)) which declares and defines a unit test for each method in the public API. That file can also be used as some kind of "integration test" if compiled using the preprocessor flag `FANCY_STRING_TEST_LOOP_ENABLED=1`. The [Makefile](./Makefile) exposes recipes for running both tests; i.e., `make test` (for the unit tests) and `make test_integration` (for the integration test, which runs as a loop with short pauses such that it is possible to look at the test's memory usage using external tools).
* [LICENSE](./LICENSE) — A file containing the license and copyright information for this project.
* [Makefile](./Makefile) — A `Makefile` (for use with [GNU Make](https://www.gnu.org/software/make/)), which is provided as a convenience, and which can be used to automation operations such as building the library, running the examples and the tests, building the API documentation website, and installing/uninstalling the library on the target system. You may run `make` or `make help` for a list of all relevant recipes.
* [VERSION](./VERSION) — A simple text file that contains the library's current version. This is used by the [Makefile](./Makefile) to generate the documentation website and to "suffix" the library binaries with the current version number.

## Disclaimer

Although I have carefully unit-tested each and every method in this library, I still consider it (the library) to be in an experimental stage. So please use it with caution, and make sure to carefully profile your application for memory leaks before pushing it to production.

## Contact

If you have any questions, if you find bugs, or if you have suggestions for this project, please feel free to contact me by opening an issue on the [repository](https://github.com/BB-301/c-fancy-string/issues).

## License

This project is released under the [MIT License](./LICENSE).

## Copyright

Copyright (c) 2023 BB-301 (fw3dg3@gmail.com)