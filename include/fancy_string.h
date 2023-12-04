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

/**
 * @file
 */

#ifndef __FANCY_STRING_H__
#define __FANCY_STRING_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// -----------------------------------------------
//                  OPAQUE TYPES
// -----------------------------------------------

/**
 * @brief An opaque type that serves as a container for the string data and which is
 * passed to the library's "string" methods for state interrogation and manipulation.
 * @note Since the \ref fancy_string_t type is opaque, the only
 * way to work with it is through a pointer whose memory
 * has been heap-allocated by a factory method such as \ref fancy_string_create().
 * @warning Once the string object is no longer
 * needed, it is the application's job to call \ref fancy_string_destroy() to free
 * the memory that is referenced internally.
 * @see fancy_string_destroy
 */
typedef struct fancy_string_s fancy_string_t;

/**
 * @brief An opaque type that serves as a container for a list of \ref fancy_string_t
 * instances and which is passed to the library's "array" methods for state interrogation
 * and manipulation.
 * @note Since the \ref fancy_string_array_t type is opaque, the only
 * way to work with it is through a pointer whose memory
 * has been heap-allocated by a factory method such as \ref fancy_string_array_create().
 * @warning Once the array object is no longer needed, it is the application's job to call
 * \ref fancy_string_array_destroy() to destroy all of the internally referenced string
 * objects and free the internal pointer list's memory.
 * @see fancy_string_array_destroy
 */
typedef struct fancy_string_array_s fancy_string_array_t;

/**
 * @brief An opaque type that serves as a container for regular expression matching
 * results, and which is passed to the library's "regex" methods for state interrogation
 * and manipulation.
 * @note Since the \ref fancy_string_regex_t type is opaque, the only
 * way to work with it is through a pointer whose memory
 * has been heap-allocated by a factory method such as \ref fancy_string_regex_create().
 * @warning Once the regex object is no longer needed, it is the application's job to call
 * \ref fancy_string_regex_destroy() to destroy all of the internally referenced
 * objects and free the internal pointers' memory.
 * @see fancy_string_regex_destroy
 */
typedef struct fancy_string_regex_s fancy_string_regex_t;

/**
 * @brief A type (i.e., a structure), returned by the \ref fancy_string_regex_match_info_for_index()
 * method, containing information about a particular match.
 *
 * @see fancy_string_regex_match_info_for_index
 */
typedef struct fancy_string_regex_match_info_s
{
    /**
     * @brief The index (i.e., the position) of the match inside the \ref fancy_string_regex_t instance.
     *
     * @note A value of `-1` indicates that the index specified when calling \ref fancy_string_regex_match_info_for_index()
     * was out of bounds. In other words, \p index should match the value specified when calling \ref fancy_string_regex_match_info_for_index(),
     * provided that that value was not out of bounds.
     */
    ssize_t index;
    /**
     * @brief The 'start' position of the match at position \p index .
     *
     */
    size_t start;
    /**
     * @brief The 'end' position of the match at position \p index .
     *
     * @note This value has a '1-position offset (to the right)' from the last matched
     * character. For instance, let's say that we had the string 'This is a test.', and
     * that we matched ' is ', we will get `{ .start = 4, .end = 8 }`.
     */
    size_t end;
} fancy_string_regex_match_info_t;

/**
 * @brief An enumeration whose members can be used to specify, through the \ref fancy_string_memory_usage_init()
 * static method, the memory usage tracking mode to be used by the library. By default,
 * \p FANCY_STRING_MEMORY_USAGE_MODE_NONE is used, such that memory tracking is not enabled.
 * @warning The memory usage tracking feature is meant to be used during development only, as a tool
 * for aiding in tracking down potential memory leaks linked to bad library usage (i.e., not destroying
 * objects before they go out of scope). This feature has not been optimized and will incur substantial
 * overhead when enabled (even if optimized, there would still be incurred overhead).
 * @see fancy_string_memory_usage_init
 */
typedef enum fancy_string_memory_usage_mode_e
{
    /**
     * @brief Disables the library's memory usage tracking feature. This is the default behaviour.
     *
     */
    FANCY_STRING_MEMORY_USAGE_MODE_NONE,
    /**
     * @brief Tracks memory usage and stores the tracking data in a private static variable for
     * each thread using the library.
     * @note This mode should be preferred to `FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC` for
     * single threaded application, since there is less overhead associated with it (the latter
     * uses a mutex each time library memory is allocated, re-allocated, and freed throughout the whole
     * process).
     * @warning When this mode is used, the library objects should not be shared between threads,
     * else application crashes will occur.
     */
    FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL,
    /**
     * @brief Tracks memory usage and stores the tracking data in a private static variable shared
     * by all threads. Internally, synchronization is achieved using `pthread_mutex_t`.
     * @note Of the three (two) modes, this mode is that one that has the most overhead associated with
     * it, because all of the process' memory is allocated, re-allocated, and freed goes through a mutex
     * lock for the whole process.
     */
    FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC
} fancy_string_memory_usage_mode_t;

// -----------------------------------------------
//                     MISC
// -----------------------------------------------

/**
 * @brief Populates its arguments with the library's version.
 *
 * @param major A pointer used to store the major version number.
 * @param minor A pointer used to store the minor version number.
 * @param revision A pointer used to store the revision number.
 *
 * @see fancy_string_library_version_print
 */
void fancy_string_library_version(uint16_t *major, uint16_t *minor, uint16_t *revision);

/**
 * @brief Prints (i.e., writes) the library's version to the specified \p stream.
 * @param stream The stream to which to write the library version (e.g. `stdout`).
 * @see fancy_string_library_version
 */
void fancy_string_library_version_print(FILE *stream);

/**
 * @brief A "static method" that retrieves and returns the amount of memory allocated by the library.
 * @note - If \ref fancy_string_memory_usage_mode_t was set to `FANCY_STRING_MEMORY_USAGE_MODE_NONE`, the returned
 * value will alway be `0`.
 * @note - If \ref fancy_string_memory_usage_mode_t was set to `FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL`,
 * the returned value will be the total memory used for the "calling thread".
 * @note - If \ref fancy_string_memory_usage_mode_t was set to `FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC`,
 * the returned value will be the total memory used for the whole process, regardless of who is the
 * "calling thread".
 * @return The total amount of memory (in bytes) currently in use by the library (not including the
 * memory used by the memory tracking mechanism, however; only for the string, array, and regular
 * expression objects).
 */
size_t fancy_string_memory_usage_get(void);

/**
 * @brief A "static method" that can be used to "print" a summary of the library's memory usage.
 * @note - If \ref fancy_string_memory_usage_mode_t was set to `FANCY_STRING_MEMORY_USAGE_MODE_NONE`, no summary
 * will be printed (only a warning message).
 * @note - If \ref fancy_string_memory_usage_mode_t was set to `FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL`,
 * a summary of the memory used for the "calling thread" will be printed.
 * @note - If \ref fancy_string_memory_usage_mode_t was set to `FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC`,
 * a summary of the memory used for the whole process (regardless of the "calling thread") will be printed.
 * @param stream A pointer to a valid readable stream to which the summary will be printed (i.e., written).
 */
void fancy_string_memory_usage_debug(FILE *stream);

/**
 * @brief If called, specifies the memory usage tracking mode to be used by the library.
 * @note - This method does not need to be called if tracking is not required. By default,
 * the library uses `FANCY_STRING_MEMORY_USAGE_MODE_NONE`, so calling the method with the
 * latter value is equivalent to doing nothing.
 * @note - If tracking is required, this method should be called (only once) as soon as possible,
 * before any library objects are created. The two tracking options are `FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL`
 * and `FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC`. See \ref fancy_string_memory_usage_mode_t for more information
 * on what these modes do.
 * @param mode A variable of type \ref fancy_string_memory_usage_mode_t , which specifies
 * the tracking mode to be used.
 * @see fancy_string_memory_usage_mode_t
 * @warning This method should be called at most once per process, else the application will likely crash.
 * In fact, calling the method any number of times with `FANCY_STRING_MEMORY_USAGE_MODE_NONE` will do
 * nothing, but once either `FANCY_STRING_MEMORY_USAGE_MODE_THREAD_LOCAL` or `FANCY_STRING_MEMORY_USAGE_MODE_GLOBAL_SYNC`
 * have been set, the method will crash if called one more time.
 * @par Example:
 * @include examples/fancy_string_memory_tracking_multi_thread.c
 */
void fancy_string_memory_usage_init(fancy_string_memory_usage_mode_t mode);

/**
 * @brief Retrieves and returns the current memory usage mode used by the library, for the
 * current process..
 *
 * @return \ref fancy_string_memory_usage_mode_t A value that corresponds to the memory tracking
 * mode used by the library for the current process.
 */
fancy_string_memory_usage_mode_t fancy_string_memory_usage_mode(void);

// -----------------------------------------------
//                 STRING (methods)
// -----------------------------------------------

/**
 * @brief Instantiates a string object with its internal state
 * specified by the \p value parameter.
 * @param value The initial state of the string object.
 * @return \ref fancy_string_t* A pointer to the created string object.
 * @see fancy_string_create_empty, fancy_string_create_repeat, fancy_string_from_stream
 */
fancy_string_t *fancy_string_create(char const *const value);

/**
 * @brief Instantiates a string object and fills its internal state
 * with the string data read from \p stream .
 * @param stream A pointer to readable stream (e.g. a file descriptor that has
 * been opened for reading).
 * @return \ref fancy_string_t* A pointer to the created string object.
 * @note Internally, this method calls \ref fgetc() repeatedly until `EOF` is reached,
 * after which it calls \ref fclose to close the stream. That method assumes that \p value
 * has been opened for reading.
 * @see fancy_string_from_stream_next_line, fancy_string_create
 * @warning Trying to read from \p stream if the latter has not been opened with
 * read permission will crash the application.
 */
fancy_string_t *fancy_string_from_stream(FILE *stream);

/**
 * @brief Reads the next line from the \p stream and
 * instantiates a string object with internal state set as the "line" that
 * was read.
 * @param stream A pointer to a readable stream from which to get the
 * next line.
 * @return \ref fancy_string_t* A pointer to the created string object which
 * contains a copy of the value read on \p stream , or the \ref NULL pointer if
 * the passed \p stream argument was \ref NULL or \ref getline(), which
 * is used internally to read the next line, returned `-1`.
 * @see fancy_string_from_stream, fancy_string_create
 * @note Unlike \ref fancy_string_from_stream(), this method will not close the
 * stream before returning.
 * @warning - Trying to read from \p stream if the latter has not been opened with
 * read permission will crash the application.
 * @warning - Internally, this method makes calls to \ref getc_unlocked() until
 * \ref EOF is returned, but the method does not bother setting nor checking (and resetting)
 * \ref errno before and after calling \ref getc_unlocked().
 * @par Example:
 * @include examples/fancy_string_from_stream_next_line.c
 */
fancy_string_t *fancy_string_from_stream_next_line(FILE *stream);

/**
 * @brief Similarly to \ref fancy_string_create(), instantiates
 * a string object, but repeats the initial value the specified number
 * of times (i.e., \p n_repeat times).
 * @param value The initial state of the string object.
 * @param n_repeat The number of times to repeat the string object.
 * @return \ref fancy_string_t* A pointer to the created string object.
 * @see fancy_string_create, fancy_string_create_empty, fancy_string_from_stream
 */
fancy_string_t *fancy_string_create_repeat(char const *const value, size_t n_repeat);

/**
 * @brief Similarly to \ref fancy_string_create(), instantiates
 * a string object, but with its internal value set to be empty.
 * @return \ref fancy_string_t* A pointer to the created string object.
 * @see fancy_string_create, fancy_string_create_repeat, fancy_string_from_stream
 */
fancy_string_t *fancy_string_create_empty(void);

/**
 * @brief Creates a memory-independent copy of the \ref fancy_string_t instance (i.e., \p self).
 * @param self A pointer to the \ref fancy_string_t instance to be cloned.
 * @return \ref fancy_string_t* A memory-independent copy of \p self.
 */
fancy_string_t *fancy_string_clone(fancy_string_t const *const self);

/**
 * @brief Destroys the string object; i.e., frees the memory that was
 * allocated for the internal string data.
 * @param self A pointer to the \ref fancy_string_t instance to be destroyed.
 *
 * @warning This method should only be called once per object, else a double-free error will incur.
 */
void fancy_string_destroy(fancy_string_t *const self);

/**
 * @brief Prints (i.e., writes) the string object's data to the specified \p stream .
 * @param self A pointer to the \ref fancy_string_t instance to be printed.
 * @param stream The stream to which to write (e.g., `stdout`).
 * @param debug Whether to use the debug format (`true`) or not (`false`). With the debug
 * format, additional information about the object and size is printed, while the standard
 * format simply writes the string data as is.
 * @see fancy_string_array_print
 */
void fancy_string_print(fancy_string_t const *const self, FILE *stream, bool debug);

/**
 * @brief Returns the size of the internal string data.
 * @param self A pointer to the \ref fancy_string_t instance for which the size is needed.
 * @return \ref size_t A value corresponding to the size of the internal string data.
 * @note Internally, C's `strlen` function is called on the string.
 */
size_t fancy_string_size(fancy_string_t const *const self);

/**
 * @brief Checks whether the string is empty.
 * @param self A pointer to the \ref fancy_string_t instance to be checked for emptiness.
 * @return \ref bool A value that is `true` if the string is empty (else it is `false`).
 */
bool fancy_string_is_empty(fancy_string_t const *const self);

/**
 * @brief Clears the internal string data.
 * @param self A pointer to the \ref fancy_string_t instance to be cleared.
 *
 * @note This method can be called multiple times without problem.
 */
void fancy_string_clear(fancy_string_t *const self);

/**
 * @brief Updates the internal state of the string object with the new string data (i.e., \p value).
 * @param self A pointer to the \ref fancy_string_t instance whose value needs updating.
 * @param value The value with which to replace the string object's internal state.
 *
 * @note The method makes a copy of the \p value, so if that \p value has been heap-allocated by the application,
 * the application is responsible for freeing its memory when no longer needed.
 *
 * @see fancy_string_update
 */
void fancy_string_update_value(fancy_string_t *const self, char const *const value);

/**
 * @brief Updates the internal state of the string object using that of another string object (i.e., \p string).
 * @param self A pointer to the \ref fancy_string_t instance whose value needs updating.
 * @param string A pointer to another \ref fancy_string_t instance whose internal state is to be copied inside \p self.
 *
 * @see fancy_string_update_value
 */
void fancy_string_update(fancy_string_t *const self, fancy_string_t const *const string);

/**
 * @brief Returns a pointer to a heap-allocated copy of the string object's internal state.
 * @param self A pointer to the \ref fancy_string_t instance for which a copy of the internal state is requested.
 * @return \ref char* A pointer to a heap-allocated copy of the string object's internal state.
 *
 * @warning Calling this method creates an heap-alloacted copy of the string object's internal
 * state, such that it becomes the application's responsibility to free that memory (using the
 * stdlib's `free` function) once it's no longer needed.
 */
char *fancy_string_value(fancy_string_t const *const self);

/**
 * @brief Appends a value to the current string object's internal state.
 * @param self A pointer to the \ref fancy_string_t instance to which to append \p value.
 * @param value The value to be appended to the internal string data.
 *
 * @note The \p value gets copied.
 *
 * @see fancy_string_append
 */
void fancy_string_append_value(fancy_string_t *const self, char const *const value);

/**
 * @brief Appends a string object's data to the current string object's internal state.
 * @param self A pointer to the \ref fancy_string_t instance to which to append.
 * @param string A pointer to another \ref fancy_string_t instance whose internal data is
 * to be appended to \p self.
 *
 * @note \p string 's data gets copied.
 *
 * @see fancy_string_append_value
 */
void fancy_string_append(fancy_string_t *const self, fancy_string_t const *const string);

/**
 * @brief Prepends a value to the current string object's internal state.
 * @param self A pointer to the \ref fancy_string_t instance to which to prepend \p value.
 * @param value The value to be prepended to the internal string data.
 *
 * @note The \p value gets copied.
 *
 * @see fancy_string_prepend
 */
void fancy_string_prepend_value(fancy_string_t *const self, char const *const value);

/**
 * @brief Prepends a string object's data to the current string object's internal state.
 * @param self A pointer to the \ref fancy_string_t instance to which to prepend.
 * @param string A pointer to another \ref fancy_string_t instance whose internal data is
 * to be prepended to \p self.
 *
 * @note \p string 's data gets copied.
 *
 * @see fancy_string_prepend_value
 */
void fancy_string_prepend(fancy_string_t *const self, fancy_string_t const *const string);

/**
 * @brief Checks whether the string object's internal value equals to \p value.
 * @param self A pointer to the \ref fancy_string_t instance for which to check for equality.
 * @param value The value against which to check for equality.
 * @return \ref bool A value indicating whether equality was found (`true`) or not (`false`).
 * @see fancy_string_equals
 */
bool fancy_string_equals_value(fancy_string_t const *const self, char const *const value);

/**
 * @brief Checks whether the string object's internal value equals to \p string 's internal value.
 * @param self A pointer to the \ref fancy_string_t instance for which to check for equality.
 * @param string A pointer to another \ref fancy_string_t instance against which to check for equality
 * @return \ref bool A value indicating whether equality was found (`true`) or not (`false`).
 * @see fancy_string_equals_value
 */
bool fancy_string_equals(fancy_string_t const *const self, fancy_string_t const *const string);

/**
 * @brief Checks whether the string object's internal value starts with \p value.
 * @param self A pointer to the \ref fancy_string_t instance to be checked.
 * @param value A value against which to check for "leading equality".
 * @return \ref bool A value indicating whether the object's data starts with \p value (`true`)
 * or not (`false`).
 * @see fancy_string_starts_with
 * @par Example:
 * @code{.c}
 * #include <assert.h>
 * #include "fancy_string.h"
 *
 * int main(void)
 * {
 *     fancy_string_t *s = fancy_string_create("Hello, World!");
 *     assert(fancy_string_starts_with_value(s, "Hello"));
 *     fancy_string_destroy(s);
 *     return 0;
 * }
 * @endcode
 */
bool fancy_string_starts_with_value(fancy_string_t const *const self, char const *const value);

/**
 * @brief Checks whether the string object's internal value starts with \p string 's internal value.
 * @param self A pointer to the \ref fancy_string_t instance to be checked.
 * @param string A Pointer to another \ref fancy_string_t instance against whose internal value to check for
 * "leading equality".
 * @return \ref bool A value indicating whether the object's data starts with \p string 's internal value (`true`)
 * or not (`false`).
 * @see fancy_string_starts_with_value
 */
bool fancy_string_starts_with(fancy_string_t const *const self, fancy_string_t const *const string);

/**
 * @brief Checks whether the string object's internal value ends with \p value.
 * @param self A pointer to the \ref fancy_string_t instance to be checked.
 * @param value A value against which to check for "trailing equality".
 * @return \ref bool A value indicating whether the object's data ends with \p value (`true`)
 * or not (`false`).
 * @see fancy_string_ends_with
 * @par Example:
 * @code{.c}
 * #include <assert.h>
 * #include "fancy_string.h"
 *
 * int main(void)
 * {
 *     fancy_string_t *s = fancy_string_create("Hello, World!");
 *     assert(fancy_string_ends_with_value(s, "World!"));
 *     fancy_string_destroy(s);
 *     return 0;
 * }
 * @endcode
 */
bool fancy_string_ends_with_value(fancy_string_t const *const self, char const *const value);

/**
 * @brief Checks whether the string object's internal value ends with \p string 's internal value.
 * @param self A pointer to the \ref fancy_string_t instance to be checked.
 * @param string A Pointer to another \ref fancy_string_t instance against whose internal value to check for
 * "trailing equality".
 * @return \ref bool A value indicating whether the object's data ends with \p string 's internal value (`true`)
 * or not (`false`).
 * @see fancy_string_ends_with_value
 */
bool fancy_string_ends_with(fancy_string_t const *const self, fancy_string_t const *const string);

/**
 * @brief Finds and returns the index of the first occurrence of \p value in the string object's
 * internal value, if any.
 *
 * @param self A pointer to the \ref fancy_string_t instance for which to find the first matching index
 * for \p value.
 * @param value The value whose first occurrence is being sought inside the string object's data.
 * @return \ref ssize_t The index of the first match, if any. If none, `-1` will be returned.
 * @see fancy_string_index_of
 */
ssize_t fancy_string_index_of_value(fancy_string_t const *const self, char const *const value);

/**
 * @brief Finds and returns the index of the first occurrence of \p string 's internal value
 * in \p self 's internal value, if any.
 *
 * @param self A pointer to the \ref fancy_string_t instance for which to find the first matching index
 * for \p string 's internal value.
 * @param string A pointer to another \ref fancy_string_t instance for whose internal value
 * is being sought inside \p self 's internal data.
 * @return \ref ssize_t The index of the first match, if any. If none, `-1` will be returned.
 * @see fancy_string_index_of_value
 */
ssize_t fancy_string_index_of(fancy_string_t const *const self, fancy_string_t const *const string);

/**
 * @brief Checks whether the string object's data contains at least one occurrence of \p value.
 * @param self A pointer to the \ref fancy_string_t instance to check.
 * @param value The value for which to check inside \p self .
 * @return \ref bool A value indicating whether \p value was found (`true`) or not (`false`).
 * @see fancy_string_contains
 */
bool fancy_string_contains_value(fancy_string_t const *const self, char const *const value);

/**
 * @brief Checks whether the string object's data contains at least one occurrence of \p string 's
 * internal value.
 * @param self A pointer to the \ref fancy_string_t instance to check.
 * @param string A pointer to another \ref fancy_string_t instance whose internal value
 * is searched inside \p self 's value.
 * @return \ref bool A value indicating whether a match was found (`true`) or not (`false`).
 * @see fancy_string_contains_value
 */
bool fancy_string_contains(fancy_string_t const *const self, fancy_string_t const *const string);

/**
 * @brief Creates a substring of a string object.
 * @param self A pointer to the \ref fancy_string_t instance for which to create a substring.
 * @param start The start index of the substring. If this value is set to `-1`, the start index will be `0`.
 * @param end The end index of the substring. If this value is set to `-1`, the end index will be `n`, which corresponds
 * to \p self 's size. It should be noted that \p end is not included in the range. For instance, `start = 0` and `end = 5`
 * will return of substring for the `0..4`, not `0..5`.
 * @return \ref fancy_string_t* A pointer to a memory-independent string object containing the specified
 * substring.
 * @note This method is very permissive with its \p start and \p end index values. For instance, if \p start is greater
 * than \p end, the two values will be swapped. Also, if \p end is out of bounds, the substring's upper bound
 * will be set as `n - 1`. Finally, if the specified range is completely out of bounds, an empty string object
 * will be returned.
 * @par Example:
 * @code{.c}
 * #include <assert.h>
 * #include "fancy_string.h"
 *
 * int main(void) {
 *     fancy_string_t *s = fancy_string_create("this is a test");
 *     // end = 6 to get 0..5 range
 *     fancy_string_t *s_sub = fancy_string_substring(s, 0, 6);
 *     assert(fancy_string_equals_value(s_sub, "this i"));
 *     fancy_string_destroy(s_sub);
 *     fancy_string_destroy(s);
 *     return 0;
 * }
 * @endcode
 */
fancy_string_t *fancy_string_substring(fancy_string_t const *const self, ssize_t start, ssize_t end);

/**
 * @brief Trims (i.e., remove all the white spaces) the left-hand side of the string object's internal value.
 * @param self A pointer to the \ref fancy_string_t instance to be trimmed.
 * @see fancy_string_trimmed_left
 */
void fancy_string_trim_left(fancy_string_t *const self);

/**
 * @brief Creates a left-hand-trimmed version of the
 * string object (i.e., a version with all the leading white spaces removed).
 * @param self A pointer to the \ref fancy_string_t instance for which to create a trimmed copy.
 * @return \ref fancy_string_t* A pointer to a new, memory-independent string object that
 * is the same as \p self, except that it has been trimmed at the left.
 * @see fancy_string_trim_left.
 */
fancy_string_t *fancy_string_trimmed_left(fancy_string_t const *const self);

/**
 * @brief Trims (i.e., remove all the white spaces) the right-hand side of the string object's internal value.
 * @param self A pointer to the \ref fancy_string_t instance to be trimmed.
 * @see fancy_string_trimmed_right
 */
void fancy_string_trim_right(fancy_string_t *const self);

/**
 * @brief Creates a right-hand-trimmed version of the
 * string object (i.e., a version with all the trailing white spaces removed).
 * @param self A pointer to the \ref fancy_string_t instance for which to create a trimmed copy.
 * @return \ref fancy_string_t* A pointer to a new, memory-independent string object that
 * is the same as \p self, except that it has been trimmed at the right.
 * @see fancy_string_trim_right.
 */
fancy_string_t *fancy_string_trimmed_right(fancy_string_t const *const self);

/**
 * @brief Trims (i.e., remove all the white spaces) both the right and left-hand sides of the
 * string object's internal values.
 * @param self A pointer to the \ref fancy_string_t instance to be trimmed.
 * @see fancy_string_trimmed
 */
void fancy_string_trim(fancy_string_t *const self);

/**
 * @brief Creates a trimmed version of the
 * string object (i.e., a version with all the leading and trailing white spaces removed).
 * @param self A pointer to the \ref fancy_string_t instance for which to create a trimmed copy.
 * @return \ref fancy_string_t* A pointer to a new, memory-independent string object that
 * is the same as \p self, except that it has been trimmed at both the right and the left.
 * @see fancy_string_trim.
 */
fancy_string_t *fancy_string_trimmed(fancy_string_t const *const self);

/**
 * @brief Splits the string into an array (i.e., a list) of string objects based on the \p separator .
 *
 * @param self A pointer to the \ref fancy_string_t instance whose internal string is to
 * be split.
 * @param separator A string that contains the pattern to be used for splitting.
 * @param n_max_splits The maximum number of times to split (i.e., the maximum number of matches to
 * act upon). If this value set `-1`, there will be as many splits as there are matches found. If set
 * to `0`, the method will simply return an array object with a single element containing a copy of the
 * original string object.
 * @return \ref fancy_string_array_t* A pointer to a newly created, memory-independent array
 * object containing the "splitted" string components.
 *
 * @see fancy_string_split
 */
fancy_string_array_t *fancy_string_split_by_value(fancy_string_t const *const self, char const *const separator, ssize_t n_max_splits);

/**
 * @brief Splits the string into an array (i.e., a list) of string objects based on the \p separator string object.
 *
 * @param self A pointer to the \ref fancy_string_t instance whose internal string is to
 * be split.
 * @param separator A pointer to the \ref fancy_string_t instance whose internal data is to
 * be used as the splitting pattern.
 * @param n_max_splits The maximum number of times to split (i.e., the maximum number of matches to
 * act upon). If this value set `-1`, there will be as many splits as there are matches found. If set
 * to `0`, the method will simply return an array object with a single element containing a copy of the
 * original string object.
 * @return \ref fancy_string_array_t* A pointer to a newly created, memory-independent array
 * object containing the "splitted" string components.
 *
 * @see fancy_string_split_by_value
 */
fancy_string_array_t *fancy_string_split(fancy_string_t const *const self, fancy_string_t const *const separator, ssize_t n_max_splits);

/**
 * @brief Pads the left-hand side of the string with the specified character (i.e., \p value ).
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be left-padded.
 * @param target_size The target size of the string after the padding.
 * @param value The character to be used for the padding.
 * @see fancy_string_padded_start
 *
 * @par Example:
 * @include examples/fancy_string_pad_start.c
 */
void fancy_string_pad_start(fancy_string_t *const self, size_t target_size, char value);

/**
 * @brief Creates a string object whose left-hand side has been padded with the specified
 * character (i.e., \p value ).
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be copied
 * and then left-padded.
 * @param target_size The target size of the padded string object.
 * @param value The character to be used for the padding.
 * @return \ref fancy_string_t* A pointer to a newly created, memory-independent, left-padded string object.
 * @see fancy_string_pad_start
 */
fancy_string_t *fancy_string_padded_start(fancy_string_t const *const self, size_t target_size, char value);

/**
 * @brief Pads the right-hand side of the string with the specified character (i.e., \p value).
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be right-padded.
 * @param target_size The target size of the string after the padding.
 * @param value The character to be used for the padding.
 * @see fancy_string_padded_end
 */
void fancy_string_pad_end(fancy_string_t *const self, size_t target_size, char value);

/**
 * @brief Creates a string object whose right-hand side has been padded with the specified
 * character (i.e., \p value).
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be copied
 * and then right-padded.
 * @param target_size The target size of the padded string object.
 * @param value The character to be used for the padding.
 * @return \ref fancy_string_t* A pointer to a newly created, memory-independent, right-padded string object.
 * @see fancy_string_pad_end
 */
fancy_string_t *fancy_string_padded_end(fancy_string_t const *const self, size_t target_size, char value);

/**
 * @brief Replaces the specified number of occurrences (i.e., \p replace_n) of \p old_value with \p new_value
 * in the string object's internal data.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be modified.
 * @param old_value A pointer to the pattern to be found and replaced with \p new_value.
 * @param new_value A pointer to the value with which to replace occurrences of \p old_value.
 * @param replace_n The maximum number of times, starting from the beginning of the string, to replace \p old_value. Specifying
 * `replace_n = -1` will result in all occurrences being replaced.
 * @see fancy_string_replace fancy_string_replaced_value fancy_string_replaced
 */
void fancy_string_replace_value(fancy_string_t *const self, char const *const old_value, char const *const new_value, ssize_t replace_n);

/**
 * @brief Replaces the specified number of occurrences (i.e., \p replace_n) of \p old_substring 's internal
 * value with \p new_substring 's internal value in the string object's internal data.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be modified.
 * @param old_substring A pointer to another \ref fancy_string_t instance containing the pattern to be found and replaced with \p new_substring 's value.
 * @param new_substring A pointer to another \ref fancy_string_t instance containing the value with which to replace occurrences of \p old_substring 's value.
 * @param replace_n The maximum number of times, starting from the beginning of the string, to replace \p old_substring 's value. Specifying
 * `replace_n = -1` will result in all occurrences being replaced.
 * @see fancy_string_replace_value fancy_string_replaced_value fancy_string_replaced
 */
void fancy_string_replace(fancy_string_t *const self, fancy_string_t const *const old_substring, fancy_string_t const *const new_substring, ssize_t replace_n);

/**
 * @brief Creates a string object with the specified number of occurrences
 * (i.e., \p replace_n) of \p old_value replaced with \p new_value.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be copied
 * and then modified.
 * @param old_value A pointer to the pattern to be found and replaced with \p new_value.
 * @param new_value A pointer to the value with which to replace occurrences of \p old_value.
 * @param replace_n The maximum number of times, starting from the beginning of the string, to replace \p old_value. Specifying
 * `replace_n = -1` will result in all occurrences being replaced.
 * @return \ref fancy_string_t* A pointer to a newly created, memory-independent string object where \p self 's occurrences
 * of \p new_value have been replaced with \p old_value.
 * @see fancy_string_replace_value fancy_string_replace fancy_string_replaced
 */
fancy_string_t *fancy_string_replaced_value(fancy_string_t const *const self, char const *const old_value, char const *const new_value, ssize_t replace_n);

/**
 * @brief Creates a string object with the specified number of occurrences
 * (i.e., \p replace_n) of \p old_substring 's internal value replaced with \p new_substring 's internal value.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is to be copied
 * and then modified.
 * @param old_substring A pointer to the pattern to be found and replaced with \p new_substring.
 * @param new_substring A pointer to the value with which to replace occurrences of \p old_substring.
 * @param replace_n The maximum number of times, starting from the beginning of the string, to replace \p old_substring. Specifying
 * `replace_n = -1` will result in all occurrences being replaced.
 * @return \ref fancy_string_t* A pointer to a newly created, memory-independent string object where \p self 's occurrences
 * of \p new_substring have been replaced with \p old_substring.
 * @see fancy_string_replace_value fancy_string_replace fancy_string_replaced_value
 */
fancy_string_t *fancy_string_replaced(fancy_string_t const *const self, fancy_string_t const *const old_substring, fancy_string_t const *const new_substring, ssize_t replace_n);

/**
 * @brief Applies a lowercase transformation to the string object's internal data.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is
 * to be lowercased.
 * @note Internally, this method uses the \ref tolower() function defined in the C Standard
 * Library `ctype.h` header file. This means, among other things, that lowercasing of accented
 * characters may not behave as expected. For instance, depending on the implementation, a character
 * such as `À` will likely not be transformed to `à`.
 * @see fancy_string_lowercased
 */
void fancy_string_lowercase(fancy_string_t *const self);

/**
 * @brief Creates a new string object whose internal data corresponds
 * to a copy of \p self with a lowercase transformation performed on it.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is
 * to be copied and then lowercased.
 * @return \ref fancy_string_t* A pointer to a newly created, memory-independent string object
 * that contains a lowercased version of \p self 's internal data.
 * @note Internally, this method uses the \ref tolower() function defined in the C Standard
 * Library `ctype.h` header file. This means, among other things, that lowercasing of accented
 * characters may not behave as expected. For instance, depending on the implementation, a character
 * such as `À` will likely not be transformed to `à`.
 * @see fancy_string_lowercase
 */
fancy_string_t *fancy_string_lowercased(fancy_string_t const *const self);

/**
 * @brief Applies an uppercase transformation to the string object's internal data.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is
 * to be uppercased.
 * @note Internally, this method uses the \ref toupper() function defined in the C Standard
 * Library `ctype.h` header file. This means, among other things, that uppercasing of accented
 * characters may not behave as expected. For instance, depending on the implementation, a character
 * such as `à` will likely not be transformed to `À`.
 * @see fancy_string_uppercased
 */
void fancy_string_uppercase(fancy_string_t *const self);

/**
 * @brief Creates a new string object whose internal data corresponds
 * to a copy of \p self with an uppercase transformation performed on it.
 * @param self A pointer to the \ref fancy_string_t instance whose internal data is
 * to be copied and then uppercased.
 * @return \ref fancy_string_t* A pointer to a newly created, memory-independent string object
 * that contains an uppercased version of \p self 's internal data.
 * @note Internally, this method uses the \ref toupper() function defined in the C Standard
 * Library `ctype.h` header file. This means, among other things, that uppercasing of accented
 * characters may not behave as expected. For instance, depending on the implementation, a character
 * such as `à` will likely not be transformed to `À`.
 * @see fancy_string_uppercase
 */
fancy_string_t *fancy_string_uppercased(fancy_string_t const *const self);

/**
 * @brief Instantiates a regular expression object and returns a pointer to it. That object can then
 * be interrogated using the various `fancy_string_regex`-prefixed methods exposed by this library.
 * @param string A pointer to a \ref fancy_string_t instance containing the string data to be searched.
 * @param pattern A pointer to a \ref fancy_string_t instance containing the string data corresponding to the regular
 * expression pattern for which to match.
 * @param n_max_matches A value that can be used to specify a maximum number of matches allowed during the
 * search. If set `-1`, the search will go on, from left to right, until no more matches are found.
 * @return \ref fancy_string_regex_t* A pointer to the created regular expression object. Note that if either
 * of \p string or \p pattern is empty, this method will return the \ref NULL pointer. The \ref NULL pointer will
 * also be returned if an error occurs while (internally) calling \ref regcomp() or \ref regexec().
 * @remark As mention in the description for the "returned type", the \ref NULL pointer will be returned if any
 * error is encountered while internally calling \ref regcomp() or \ref regexec(). This method currently does
 * not provide information about such errors.
 * @note - The \p string 's data is searched for \p pattern from left to right, until either \p n_max_matches
 * is reached or no more matches are found.
 * @note - Internally, this method relies on the \ref regcomp() and \ref regexec()
 * POSIX functions. \ref regcomp() is called using the `REG_EXTENDED` flag, which means
 * that the [POSIX-Extended Regular Expressions](https://en.wikibooks.org/wiki/Regular_Expressions/POSIX-Extended_Regular_Expressions)
 * syntax is used. Read more: [POSIX Regular Expression](https://www.gnu.org/software/libc/manual/html_node/POSIX-Regexp-Compilation.html)
 * @par Example:
 * @include examples/fancy_string_regex_create.c
 */
fancy_string_regex_t *fancy_string_regex_create(fancy_string_t const *const string, fancy_string_t const *const pattern, ssize_t n_max_matches);

/**
 * @brief Destroys the regular expression object.
 *
 * @param self A pointer to the \ref fancy_string_regex_t instance to be destroyed.
 */
void fancy_string_regex_destroy(fancy_string_regex_t *const self);

/**
 * @brief Prints a summary of the \p self regular expression object to the specified stream (i.e., \p stream ).
 * @param self A pointer to the \ref fancy_string_regex_t instance to be printed.
 * @param stream A pointer to a valid "readable" stream.
 * @param verbose A boolean value indicating whether (`true`) or not (`false`) to print extra information. That
 * extra information, if present, will be prefixed with the word "verbose".
 */
void fancy_string_regex_debug(fancy_string_regex_t const *const self, FILE *stream, bool verbose);

/**
 * @brief Creates and returns an array object whose internal components correspond to the
 * regular expression object's matches.
 * @param self A pointer to the \ref fancy_string_regex_t instance whose matches are to be
 * extracted and returned as an array object.
 * @return \ref fancy_string_array_t* A pointer to an array object containing the string representations
 * of the regular expression object's matches.
 */
fancy_string_array_t *fancy_string_regex_matches_to_strings(fancy_string_regex_t const *const self);

/**
 * @brief Creates and returns a new string object whose internal string data is equivalent to the
 * regular expression object's string object, except that the matches (if any) have been replaced
 * with \p new_string .
 * @param self A pointer to a \ref fancy_string_regex_t instance whose matches' values are replaced
 * with \p new_string to create a new string object based on the original one.
 * @param new_string A pointer to the \ref fancy_string_t instance whose internal string data is to
 * be used as replacement for that matches
 * @return \ref fancy_string_t* A pointer to a newly created string object that contains the same data
 * as the regular expression object's string object, except that the matched positions have been replaced
 * with \p new_string .
 * @see fancy_string_replace, fancy_string_replace_value, fancy_string_replaced, fancy_string_replaced_value
 */
fancy_string_t *fancy_string_regex_replaced_matches(fancy_string_regex_t *const self, fancy_string_t *new_string);

/**
 * @brief Uses the matches as separators for splitting the regular expression object's string into
 * an array object.
 * @param self A pointer to a \ref fancy_string_regex_t instance whose internal string object is
 * to be split into an array object.
 * @return \ref fancy_string_array_t* A pointer to an array object created by splitting the regular
 * expression object's internal string object using the matches as separators.
 * @see fancy_string_split, fancy_string_split_by_value
 */
fancy_string_array_t *fancy_string_regex_split_at_matches(fancy_string_regex_t const *const self);

/**
 * @brief Returns the number of matches obtained for the regular expression object pointed to
 * by \p self .
 *
 * @param self A pointer to the \ref fancy_string_regex_t instance for which to retrieve
 * the number of matches found.
 * @return \ref size_t The number of matches that were found for the regular expression object \p self .
 */
size_t fancy_string_regex_match_count(fancy_string_regex_t const *const self);

/**
 * @brief Checks whether there was at least on match in the regular expression object
 * pointed to by \p self .
 * @param self A pointer to the \ref fancy_string_regex_t instance for which to check
 * whether at least one match was found.
 * @return \ref bool A boolean value indicating whether (`true`) or not (`false`) at
 * least one match was found for the current regular expression object.
 */
bool fancy_string_regex_has_match(fancy_string_regex_t const *const self);

/**
 * @brief Retrieves and returns the `n_max_matches` value (i.e., the maximum number of matches
 * allowed for the regular expression instance) that was used when instantiating
 * the \ref fancy_string_regex_t object (i.e., the object pointed to by \p self ).
 * @param self A pointer to the \ref fancy_string_regex_t instance for which to check the
 * specified `n_max_matches`.
 * @return \ref ssize_t The maximum number of matches that were allowed for the regular expression
 * object. A value of `-1` means that there was no limit.
 */
ssize_t fancy_string_regex_max_number_of_matches(fancy_string_regex_t const *const self);

/**
 * @brief Checks, for \p self , whether a maximum number of matches was set and, if so,
 * whether that number was reached.
 * @param self A pointer to a \ref fancy_string_regex_t instance for which to check whether
 * the maximum number of matches was reached (if specified).
 * @return \ref bool A boolean value indicating whether (`true`) or not (`false`) the maximum
 * number of matches (if specified) was reached.
 * @note When calling \ref fancy_string_regex_create(),
 * if `n_max_matches` is set to `-1`, no limit will be set, and this will alway return `false`
 * for the corresponding instance.
 */
bool fancy_string_regex_max_number_of_matches_reached(fancy_string_regex_t const *const self);

/**
 * @brief Retrieves and returns the regular expression object's content string (i.e., the string to be searched
 * for matches) as a memory-independent string object.
 * @param self A pointer to a \ref fancy_string_regex_t instance for which to retrieve the "content string".
 * @return \ref fancy_string_t* A pointer to a memory-independent string object containing a copy of the regular expression object's
 * internal "content string".
 */
fancy_string_t *fancy_string_regex_string(fancy_string_regex_t const *const self);

/**
 * @brief Retrieves and returns the regular expression object's pattern as a memory-independent
 * string object.
 * @param self A pointer to a \ref fancy_string_regex_t instance for which to retrieve the "regular expression"
 * pattern.
 * @return \ref fancy_string_t* A pointer to a memory-independent string object containing a copy of the regular expression object's
 * internal pattern.
 */
fancy_string_t *fancy_string_regex_pattern(fancy_string_regex_t const *const self);

/**
 * @brief The signature a callback function needs to implement to be allowed
 * to be passed as argument to the \ref fancy_string_regex_to_string_with_updated_matches() method.
 * @param match A pointer to the \ref fancy_string_t instance on which to operate, which
 * corresponds to the matched string at position \p start to \p end (where end is 1 position more than the
 * last character of the match) inside the regular expression object's string object.
 * @param start The start position of the match in the regular expression object's string object.
 * @param end The end position of the match in the regular expression object's string object. Note that
 * this value is "one" position more than the last character of the match.
 * @param context A pointer to the application-defined context object.
 * @see fancy_string_regex_to_string_with_updated_matches
 */
typedef void (*fancy_string_regex_updater_t)(fancy_string_t *const match, size_t start, size_t end, fancy_string_t const *const string, void *context);

/**
 * @brief Similar to, but more flexible than \ref fancy_string_regex_replaced_matches(), this method
 * can be used to generate a string object whose internal data is a copy of the regular expression object's
 * original string, but with (potentially) modified substring values where matches were founds. The reason
 * why this method is more flexible than \ref fancy_string_regex_replaced_matches() is because it allows
 * applying custom modification rules on a per-match basis (see the example).
 *
 * @param self A pointer to the \ref fancy_string_regex_t instance to be used to generate the new string object.
 * @param fn A pointer to a function that conforms to the \ref fancy_string_regex_updater_t signature, which
 * will be called internally for each match in the regular expression object \p self .
 * @param context A pointer to an application-defined context object, which gets passed to \p fn on each call.
 * @return \ref fancy_string_t* A pointer to the created string object.
 * @see fancy_string_regex_updater_t, fancy_string_regex_replaced_matches
 * @note This method is used internally by the \ref fancy_string_regex_debug() method, when its `verbose` argument
 * is set to `true`, to generate the `.verbose_string[calculated]` debug output.
 * @par Example:
 * @include examples/fancy_string_regex_to_string_with_updated_matches.c
 */
fancy_string_t *fancy_string_regex_to_string_with_updated_matches(fancy_string_regex_t const *const self, fancy_string_regex_updater_t fn, void *context);

/**
 * @brief Retrieves and returns the match information in a \ref fancy_string_regex_t instance for the
 * specified position (i.e., \p index ).
 * @param self A pointer to the \ref fancy_string_regex_t instance for which the match
 * info (i.e., \ref fancy_string_regex_match_info_t) is requested.
 * @param index The position (i.e., the index) of the match to be returned
 * @return \ref fancy_string_regex_match_info_t A structure containing the requested match information. If
 * `.index = -1`, that means that the specified position (i.e, \p index ) was out of bounds, such that both
 * `.start` and `.end` will be set to zero. If the \p index argument was valid, it will be returned in `.index`,
 * and the `.start` and `.end` values will correspond to the location of the \p index -th match in the target
 * string object (for which string object a copy can be requested using the \ref fancy_string_regex_string method on
 * \p self ).
 * @par Example:
 * @include examples/fancy_string_regex_match_info_for_index.c
 */
fancy_string_regex_match_info_t fancy_string_regex_match_info_for_index(fancy_string_regex_t const *const self, size_t index);

// -----------------------------------------------
//                  ARRAY (methods)
// -----------------------------------------------

/**
 * @brief Instantiates an empty string array object.
 * @return \ref fancy_string_array_t* A pointer to the created array object.
 */
fancy_string_array_t *fancy_string_array_create(void);

/**
 * @brief Instantiates an array object with a variable list of string values.
 *
 * @param first_value A pointer to the first string value argument to be used to
 * instantiate a memory-independent string object to be added first to the array.
 * @param ... A variable number of string pointers (i.e., `char *`) to be used, in the
 * order they appear, in the same way as \p first_value . **Important:** The last value should be the \ref NULL pointer,
 * unless \p first_value itself is \ref NULL , in which case all other arguments will be ignored and
 * an empty array will be returned.
 * @return \ref fancy_string_array_t* A pointer to the created array object containing
 * the string objects specified as arguments.
 * @warning Internally, this method is based on the macros defined by \ref stdarg.h, which means
 * that a way of knowing where to stop, when reading the variable list of argument, is needed.
 * The adopted approach here is therefore to add the \ref NULL pointer as the last argument.
 * If \ref NULL is omitted, the result will be unpredictable.
 * @see fancy_string_array_create, fancy_string_array_push_values
 * @par Example:
 * @include examples/fancy_string_array_create_with_values.c
 */
fancy_string_array_t *fancy_string_array_create_with_values(char const *const first_value, ...);

/**
 * @brief Clones a string array object.
 * @param self A pointer to the \ref fancy_string_array_t instance to be cloned.
 * @return \ref fancy_string_array_t* A pointer to the newly created, memory-independent
 * array object that is a clone of \p self.
 */
fancy_string_array_t *fancy_string_array_clone(fancy_string_array_t const *const self);

/**
 * @brief Appends the elements of another array object (i.e., \p array) into the current
 * array object (i.e., \p self).
 * @param self A pointer to the \ref fancy_string_array_t instance to which to append a copy
 * of \p array 's elements.
 * @param array A pointer to another \ref fancy_string_array_t instance whose \ref fancy_string_t
 * elements are to be copied and appended to \p self.
 * @see fancy_string_array_append_and_destroy
 */
void fancy_string_array_append(fancy_string_array_t *const self, fancy_string_array_t const *const array);

/**
 * @brief Appends the elements of another array object (i.e., \p array) into the current
 * array object (i.e., \p self), and then internally calls \ref fancy_string_array_destroy()
 * on \p array.
 * @param self A pointer to the \ref fancy_string_array_t instance to which to append a copy
 * of \p array 's elements.
 * @param array A pointer to another \ref fancy_string_array_t instance whose \ref fancy_string_t
 * elements are to be copied and appended to \p self, and which is to subsequently be destroyed.
 * @see fancy_string_array_append fancy_string_array_destroy
 */
void fancy_string_array_append_and_destroy(fancy_string_array_t *const self, fancy_string_array_t *array);

/**
 * @brief Destroys the array object; i.e., frees the memory that was
 * allocated for the internal pointers to \ref fancy_string_t instances and
 * destroys those string instances using \ref fancy_string_destroy().
 *
 * @param self A reference to the \ref fancy_string_array_t instance to be destroyed.
 *
 * @see fancy_string_destroy
 *
 * @warning This method should only be called once per object, else a double-free error will incur.
 */
void fancy_string_array_destroy(fancy_string_array_t *const self);

/**
 * @brief Prints (i.e., writes) the array's string contents to the specified \p stream .
 * @param self A pointer to the \ref fancy_string_array_t instance to be printed.
 * @param stream The stream to which to write (e.g., `stdout`).
 * @param debug Whether to use the debug format (`true`) or not (`false`). With the debug
 * format, additional information about the object and size is printed, while the standard
 * format simply writes the internal string objects data as is.
 * @see fancy_string_print
 */
void fancy_string_array_print(fancy_string_array_t const *const self, FILE *stream, bool debug);

/**
 * @brief Clears the array; i.e., destroys all of the internal \ref fancy_string_t instances
 * and frees the memory used to track those instances.
 * @param self A pointer to the \ref fancy_string_array_t instance to be cleared.
 */
void fancy_string_array_clear(fancy_string_array_t *const self);

/**
 * @brief Returns the array object's size; i.e., the number of \ref fancy_string_t instances
 * it contains.
 * @param self A pointer to the \ref fancy_string_array_t instance for which the size
 * is to be returned.
 * @return \ref size_t The number of elements (i.e., \ref fancy_string_t instances) in the array object.
 */
size_t fancy_string_array_size(fancy_string_array_t const *const self);

/**
 * @brief Checks whether the array is empty.
 * @param self A pointer to the \ref fancy_string_array_t instance to be checked for "emptiness".
 * @return \ref bool A boolean value that is `true` if the array is empty, else it is set to `false`.
 */
bool fancy_string_array_is_empty(fancy_string_array_t const *const self);

/**
 * @brief Instantiates a string object, with internal state set
 * to \p value , and appends it at the end of the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance onto which to push the new string object.
 * @param value The value with which to populate the new \ref fancy_string_t instance.
 * @see fancy_string_array_push
 */
void fancy_string_array_push_value(fancy_string_array_t *const self, char const *const value);

/**
 * @brief Instantiates a variable number of string objects with internal states set
 * to \p first_value and `...` , and appends those in order at the end of the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance at the end of which to insert the new string
 * objects.
 * @param first_value The value with which to populate the first new \ref fancy_string_t instance.
 * @param ... A variable number of string pointers (i.e., `char *`) to be used, in the
 * order they appear, in the same way as \p first_value . **Important:** The last value should be the \ref NULL pointer,
 * unless \p first_value itself is \ref NULL, in which case all other arguments will be ignored and
 * an the array object won't be modified.
 * @see fancy_string_array_create_with_values, fancy_string_array_push_value, fancy_string_array_push
 * @warning Internally, this method is based on the macros defined by \ref stdarg.h, which means
 * that a way of knowing where to stop, when reading the variable list of argument, is needed.
 * The adopted approach here is therefore to add the \ref NULL pointer as the last argument.
 * If \ref NULL is omitted, the result will be unpredictable.
 * @note See the documentation for \ref fancy_string_array_create_with_values for an example on how
 * this method can be used to insert multiple string objects at the an of an array object.
 */
void fancy_string_array_push_values(fancy_string_array_t *const self, char const *const first_value, ...);

/**
 * @brief Appends a memory-independent copy of the string object \p string at the end of the
 * array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance onto which to push a copy of \p string .
 * @param string A pointer to the \ref fancy_string_t instance which is to be copied and that copy appended to the array
 * object's internal list.
 * @see fancy_string_array_push_value
 */
void fancy_string_array_push(fancy_string_array_t *const self, fancy_string_t const *const string);

/**
 * @brief Returns a memory-independent copy of the \ref fancy_string_t instance located
 * at position \p index inside the array object.
 * @param self A pointer to the \ref fancy_string_array_t instance whose \p index -th element
 * is to be cloned and returned.
 * @param index The position (in the array object's internal list) of the sought element (i.e.,
 * \ref fancy_string_t instance).
 * @return A memory-independent copy of the \ref fancy_string_t instance at position \p index . If
 * \p index is out of bounds, then the \ref NULL pointer is returned.
 * @see fancy_string_array_get_value
 */
fancy_string_t *fancy_string_array_get(fancy_string_array_t const *const self, size_t index);

/**
 * @brief Returns a memory-independent copy of the \ref fancy_string_t instance's internal state (i.e.,
 * string data) located at position \p index inside the array object.
 * @param self A pointer to the \ref fancy_string_array_t instance whose \p index -th element is
 * to have its internal state cloned and returned.
 * @param index The position (in the array object's internal list) of the sought element (i.e.,
 * \ref fancy_string_t instance's value).
 * @return \ref char* A memory-independent copy of the \ref fancy_string_t instance's internal state
 * at position \p index . If \p index is out of bounds, then the \ref NULL pointer is returned.
 * @see fancy_string_array_get
 */
char *fancy_string_array_get_value(fancy_string_array_t const *const self, size_t index);

/**
 * @brief Removes and returns the pointer to the \ref fancy_string_t instance located
 * at position \p index inside the array object.
 * @param self A pointer to the \ref fancy_string_array_t instance whose \p index -th element
 * is to be removed and returned.
 * @param index The position (in the array object's internal list) of the sought element (i.e.,
 * the \ref fancy_string_t instance to be removed).
 * @return A pointer to the removed \ref fancy_string_t instance, if any, else the \ref NULL pointer.
 * @see fancy_string_array_remove_and_destroy
 * @warning Once removed from its original array object, the string object becomes the application's
 * responsibility, and therefore needs to be destroyed using the \ref fancy_string_destroy method
 * once it's no longer needed.
 */
fancy_string_t *fancy_string_array_remove(fancy_string_array_t *const self, size_t index);

/**
 * @brief Removes and destroys the \ref fancy_string_t instance located at position \p index inside the
 * array object.
 * @param self A pointer to the \ref fancy_string_array_t instance whose \p index -th element
 * is to be removed and destroyed.
 * @param index The position (in the array object's internal list) of the element sought for removal
 * and destruction.
 * @see fancy_string_array_remove fancy_string_destroy
 * @note If the position \p index is out of bounds, nothing will happen.
 */
void fancy_string_array_remove_and_destroy(fancy_string_array_t *const self, size_t index);

/**
 * @brief Removes the last element from the array object's internal list and returns a pointer to it.
 * @param self A pointer to the \ref fancy_string_array_t instance whose internal list's last
 * element is to be removed.
 * @return \ref fancy_string_t A pointer to the removed element, if any, else the \ref NULL pointer.
 * @see fancy_string_array_pop_and_destroy fancy_string_destroy
 * @warning Once removed from its original array object, the string object becomes the application's
 * responsibility, and therefore needs to be destroyed using the \ref fancy_string_destroy method
 * once it's no longer needed.
 */
fancy_string_t *fancy_string_array_pop(fancy_string_array_t *const self);

/**
 * @brief Removes and destroys the array object's internal list's last element.
 * @param self A pointer to the \ref fancy_string_array_t instance whose internal list's last element
 * is to be removed and destroyed.
 * @see fancy_string_array_pop
 */
void fancy_string_array_pop_and_destroy(fancy_string_array_t *const self);

/**
 * @brief Remove the first element from the array object's internal list returns a pointer to it.
 * @param self A pointer to the \ref fancy_string_array_t instance whose internal list's first
 * element is to be removed.
 * @return \ref fancy_string_t A pointer to the removed element, if any, else the \ref NULL pointer.
 * @see fancy_string_array_shift_and_destroy fancy_string_destroy
 * @warning Once removed from its original array object, the string object becomes the application's
 * responsibility, and therefore needs to be destroyed using the \ref fancy_string_destroy method
 * once it's no longer needed.
 */
fancy_string_t *fancy_string_array_shift(fancy_string_array_t *const self);

/**
 * @brief Removes and destroys the array object's internal list's first element.
 * @param self A pointer to the \ref fancy_string_array_t instance whose internal list's first element
 * is to be removed and destroyed.
 * @see fancy_string_array_shift
 */
void fancy_string_array_shift_and_destroy(fancy_string_array_t *const self);

/**
 * @brief Instantiates a \ref fancy_string_t object, with its internal state set as \p value , and inserts
 * it at the specified position \p index inside the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance into which to insert the new string object.
 * @param value The internal state of the string object to be created add inserter into the array object's
 * internal list.
 * @param index The position at which to insert the new string object into the array object's internal list.
 * @note This method is very permissive. If the insertion position \p index is out of bounds, the array will
 * be padded with empty string objects up to one less than \p index , and the string object will be added at
 * \p index as requested. If an element is present at position \p index , that element, along with all of the
 * elements to the right of it, will be moved right by one position.
 * @ see fancy_string_array_insert
 */
void fancy_string_array_insert_value(fancy_string_array_t *const self, char const *const value, size_t index);

/**
 * @brief Inserts a memory-independent copy of the string object \p string at the specified
 * position \p index inside the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance into which to insert the string object.
 * @param string A pointer to the \ref fancy_string_t instance to be cloned and inserted into the array object's
 * internal list.
 * @param index The position at which to insert the string object into the array object's internal list.
 * @note This method is very permissive. If the insertion position \p index is out of bounds, the array will
 * be padded with empty string objects up to one less than \p index , and the string object will be added at
 * \p index as requested. If an element is present at position \p index , that element, along with all of the
 * elements to the right of it, will be moved right by one position.
 * @ see fancy_string_array_insert_value
 */
void fancy_string_array_insert(fancy_string_array_t *const self, fancy_string_t const *const string, size_t index);

/**
 * @brief Returns a memory-independent copy of the array object's internal list's first string object.
 * @param self A pointer to the \ref fancy_string_array_t instance for which the first
 * element of the internal list is to be copied and returned.
 * @return \ref fancy_string_t* A pointer to a \ref fancy_string_t instance that corresponds to
 * the array object's internal list's first string object (a copy of it). If the array object is empty, the \ref NULL pointer is returned.
 * @see fancy_string_array_first_value
 */
fancy_string_t *fancy_string_array_first(fancy_string_array_t const *const self);

/**
 * @brief Returns a memory-independent copy of the array object's internal list's first string object's
 * internal string value.
 * @param self A pointer to the \ref fancy_string_array_t instance for which the first
 * element of the internal list is to have its internal string value copied and returned as a pointer.
 * @return \ref char* A pointer to string data that was copied from the first element of the array object's
 * internal list. If the array object is empty, the \ref NULL pointer is returned.
 * @see fancy_string_array_first
 */
char *fancy_string_array_first_value(fancy_string_array_t const *const self);

/**
 * @brief Returns a memory-independent copy of the array object's internal list's last string object.
 * @param self A pointer to the \ref fancy_string_array_t instance for which the last
 * element of the internal list is to be copied and returned.
 * @return \ref fancy_string_t* A pointer to a \ref fancy_string_t instance that corresponds to
 * the array object's internal list's last string object (a copy of it). If the array object is empty,
 * the \ref NULL pointer is returned.
 * @see fancy_string_array_last_value
 */
fancy_string_t *fancy_string_array_last(fancy_string_array_t const *const self);

/**
 * @brief Returns a memory-independent copy of the array object's internal list's last string object's
 * internal string value.
 * @param self A pointer to the \ref fancy_string_array_t instance for which the last
 * element of the internal list is to have its internal string value copied and returned as a pointer.
 * @return \ref char* A pointer to string data that was copied from the last element of the array object's
 * internal list. If the array object is empty, the \ref NULL pointer is returned.
 * @see fancy_string_array_last
 */
char *fancy_string_array_last_value(fancy_string_array_t const *const self);

/**
 * @brief Creates (and returns a pointer to) a \ref fancy_string_t instance and populates
 * it with copies of the array object's elements' string data, from first to last, using
 * the \p separator argument as separator.
 * @param self A pointer to the \ref fancy_string_array_t instance to be "joined" into a string object.
 * @param separator A pointer to a string to be used as separator while joining.
 * @return \ref fancy_string_t* A pointer to the memory-independent created string object.
 * @see fancy_string_array_join
 */
fancy_string_t *fancy_string_array_join_by_value(fancy_string_array_t const *const self, char const *const separator);

/**
 * @brief Creates (and returns a pointer to) a \ref fancy_string_t instance and populates
 * it with copies of the array object's elements' string data, from first to last, using
 * the \p separator argument as separator.
 * @param self A pointer to the \ref fancy_string_array_t instance to be "joined" into a string object.
 * @param separator A pointer to a \ref fancy_string_t instance whose internal string value is
 * to be used as separator while joining.
 * @return \ref fancy_string_t* A pointer to the memory-independent created string object.
 * @see fancy_string_array_join_by_value
 */
fancy_string_t *fancy_string_array_join(fancy_string_array_t const *const self, fancy_string_t const *const separator);

/**
 * @brief Reverses the order of the elements in the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance whose internal list's elements
 * order is to be reversed.
 * @see fancy_string_array_reversed
 */
void fancy_string_array_reverse(fancy_string_array_t *const self);

/**
 * @brief Clones the array object and reverse that clone before returning it.
 * @param self A pointer to the \ref fancy_string_array_t instance which is to be
 * cloned and whose clone is to have its internal list's elements order reversed.
 * @return \ref fancy_string_array_t* A pointer to the created "reversed" array object.
 * @see fancy_string_array_reverse
 */
fancy_string_array_t *fancy_string_array_reversed(fancy_string_array_t const *const self);

/**
 * @brief The signature a callback function needs to implement to be allowed
 * to be passed as argument to the \ref fancy_string_array_for_each() method.
 * @param string A pointer to the \ref fancy_string_t instance on which to operate, which
 * is found at position \p index in \p array.
 * @param index The position (i.e., index) of \p string in the \p array.
 * @param array A pointer to the \ref fancy_string_array_t instance inside of which \p string exists.
 * @param context An application-defined context object.
 * @see fancy_string_array_for_each
 */
typedef void (*fancy_string_for_each_t)(fancy_string_t *const string, size_t index, fancy_string_array_t const *const array, void *context);

/**
 * @brief Iterates over the array object's elements and calls the \p fn callback with each
 * element (i.e., \ref fancy_string_t instance), along with its position and a reference to
 * its parent array object.
 * @param self A pointer to the \ref fancy_string_array_t instance whose internal list's elements are
 * to be iterated over with the specified callback.
 * @param fn A pointer to the function to be called for each element in the array.
 * @param context An application-defined context object that will be passed to \p fn, along with
 * each individual \ref fancy_string_t instance contained in the array object's internal list.
 * @see fancy_string_for_each_t
 * @par Example:
 * @include examples/fancy_string_array_for_each.c
 */
void fancy_string_array_for_each(fancy_string_array_t *const self, fancy_string_for_each_t fn, void *context);

/**
 * @brief The signature a callback function needs to implement to be allowed
 * to be passed as argument to the \ref fancy_string_array_mapped() method.
 * @param string A pointer to the \ref fancy_string_t instance found at position \p index in \p array .
 * @param index The position of \p string in \p array .
 * @param array A pointer to the \ref fancy_string_array_t instance inside which \p string exists.
 * @param context An application-defined context object.
 * @return \ref fancy_string_t* A pointer to a string object created and returned by the callback.
 * @see fancy_string_array_mapped
 */
typedef fancy_string_t *(*fancy_string_mapped_t)(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context);

/**
 * @brief Iterates over the array object's elements and calls the \p fn callback with each
 * element (i.e, \ref fancy_string_t instance), along with its position and a reference to
 * its parent array object.
 * @param self A pointer to the \ref fancy_string_array_t instance whose internal list's elements are
 * to be iterated over with the specified callback.
 * @param fn A pointer to the function to be called for each element in the array. A call to this
 * function will return a \ref fancy_string_t instance that should be memory-independent of the
 * one that was passed in.
 * @param context An application-defined context object.
 * @return \ref fancy_string_array_t* A pointer to a new array object containing the mapped elements
 * in its internal list.
 * @see fancy_string_mapped_t
 * @warning The function implementing \ref fancy_string_mapped_t should never return a pointer to
 * the \ref fancy_string_t instance that was passed in as an argument. Compiling with the `-Werror`
 * flag, however, should prevent that from happening.
 * @par Example:
 * @include examples/fancy_string_array_mapped.c
 */
fancy_string_array_t *fancy_string_array_mapped(fancy_string_array_t const *const self, fancy_string_mapped_t fn, void *context);

/**
 * @brief The signature a callback function needs to implement to be allowed
 * to be passed as argument to the \ref fancy_string_array_sort and the \ref fancy_string_array_sorted methods.
 * @param string_1 A pointer to a \ref fancy_string_t instance to be compared to \p string_2 .
 * @param string_2 A pointer to a \ref fancy_string_t instance to be compared to \p string_1 .
 * @param context An application-defined context object.
 * @return \ref bool A boolean value that is set to `false` if \p string_1 was determined to be lesser than or
 * equal to \p string_2 , else, it is set to `true`.
 * @see fancy_string_array_sort fancy_string_array_sorted
 */
typedef bool (*fancy_string_sort_t)(fancy_string_t const *const string_1, fancy_string_t const *const string_2, void *context);

/**
 * @brief Sorts the array object's internal list's elements based on the rules established by the \p fn callback.
 * @param self A pointer to the \ref fancy_string_array_t instance to be sorted.
 * @param fn A pointer to the sorting function to be used.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @see fancy_string_sort_t fancy_string_array_sorted
 * @par Example:
 * @include examples/fancy_string_array_sort.c
 */
void fancy_string_array_sort(fancy_string_array_t *const self, fancy_string_sort_t fn, void *context);

/**
 * @brief Creates a sorted copy of \p self .
 * @param self A pointer to the \ref fancy_string_array_t instance to be cloned and then sorted.
 * @param fn A pointer to the sorting function to be used.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref fancy_string_array_t* A pointer to the memory-independent sorted copy of \p self .
 * @see fancy_string_sort_t fancy_string_array_sort
 * @note See the documentation for \ref fancy_string_array_sort for an example on how to define a
 * sorting function (to be passed to \p fn ).
 */
fancy_string_array_t *fancy_string_array_sorted(fancy_string_array_t const *const self, fancy_string_sort_t fn, void *context);

/**
 * @brief Sorts the array object's internal list's elements based on their internal values, in the most
 * basic way possible; i.e., from the smallest to greatest, as one would expect, using individual bytes
 * as a basis for comparison.
 * @param self A pointer to the \ref fancy_string_array_t instance to be sorted.
 * @see fancy_string_array_sorted_values
 * @par Example:
 * @include examples/fancy_string_array_sort_values.c
 */
void fancy_string_array_sort_values(fancy_string_array_t *const self);

/**
 * @brief Clones the array object and then sorts the clone's internal list's elements based on their
 * internal values, in the most basic way possible; i.e., from the smallest to greatest, as one would expect,
 * using individual bytes as a basis for comparison.
 * @param self A pointer to the \ref fancy_string_array_t instance to be cloned and whose clone to be sorted.
 * @return \ref fancy_string_array_t* A pointer to the created memory-independent sorted array object.
 * @see fancy_string_array_sort_values
 * @note See the documentation for \ref fancy_string_array_sort_values for an example on how to use this
 * method.
 */
fancy_string_array_t *fancy_string_array_sorted_values(fancy_string_array_t const *const self);

/**
 * @brief The signature a callback function needs to implement to be allowed to be passed
 * as argument to the \ref fancy_string_array_filter and \ref fancy_string_array_filtered methods.
 * @param string A pointer to the \ref fancy_string_t instance that is subject to being filtered in or out, which
 * instance is found at position \p index in \p array .
 * @param index The position (i.e., index) of \p string in \p array.
 * @param array A pointer to the \ref fancy_string_array_t instance inside of which \p string exists.
 * @param context An application-defined context object.
 * @return \ref bool A boolean value that is set to `true` if \p string has been retained, or set
 * to `false` if \p string has been filtered out.
 * @see fancy_string_array_filter fancy_string_array_filtered
 */
typedef bool (*fancy_string_filter_t)(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context);

/**
 * @brief Filters (i.e., updates) the array object's internal list's elements based on the rules
 * established by the \p fn callback.
 * @param self A pointer to the \ref fancy_string_array_t instance to be filtered.
 * @param fn A pointer to the filtering function to be used.
 * @param context An application-defined context object.
 * @see fancy_string_filter_t fancy_string_array_filtered
 * @note Internally, this method will call \ref fancy_string_array_remove_and_destroy on the
 * elements that have been filtered out.
 * @par Example:
 * @include examples/fancy_string_array_filter.c
 */
void fancy_string_array_filter(fancy_string_array_t *const self, fancy_string_filter_t fn, void *context);

/**
 * @brief Clones the array object (i.e., \p self ) and performs filtering on that clone, based on the rules
 * established by the \p fn callback.
 * @param self A pointer to the \ref fancy_string_array_t instance to be cloned and whose clone
 * to be sorted.
 * @param fn A pointer to the filtering function to be used.
 * @param context An application-defined context object.
 * @return \ref fancy_string_array_t* A pointer to the created, memory-independent filtered array object.
 * @see fancy_string_filter_t fancy_string_array_filter
 * @note See the documentation for the \ref fancy_string_array_filter method for an example that illustrates
 * how a filtering function (to be passed as \p fn ) can be defined.
 */
fancy_string_array_t *fancy_string_array_filtered(fancy_string_array_t const *const self, fancy_string_filter_t fn, void *context);

/**
 * @brief Creates a memory-independent slice of the array object.
 * @param self A pointer to the \ref fancy_string_array_t instance for which to generate a slice.
 * @param start The position at which the slice starts. If set to `-1`, will be interpreted as `0`.
 * @param end The position before which the slice ends. If set to `-1`, will be interpreted as
 * the array object's size (i.e., the total number of elements contained in the array object's internal list).
 * @return \ref fancy_string_array_t* A pointer to the created slice.
 * @note This method is very permissive with regards to \p start and \p end . Setting \p start to `-1` will be
 * interpreted as `0`, while setting \p end to `-1` will be interpreted as `n` (where `n` is the number of elements
 * in the array object's internal list). Setting \p start such that it is greater than \p end will simply result in those
 * two values being swapped. If \p start is out of bounds, an empty array object will be returned. If \p self is empty,
 * an empty slice will be returned, regardless of the values of \p start and \p end . If \p end is out of bounds, the
 * slice will contain a copy of every element starting at position \p start .
 * @see fancy_string_array_slice_and_destroy
 */
fancy_string_array_t *fancy_string_array_sliced(fancy_string_array_t const *const self, ssize_t start, ssize_t end);

/**
 * @brief Slices the array object \p self based on the \p start and \p end positions, preserving the elements in that range,
 * while destroying the elements falling outside of it.
 * @param self A pointer to the \ref fancy_string_array_t instance to be sliced.
 * @param start The position at which the slice starts. If set to `-1`, will be interpreted as `0`.
 * @param end The position before which the slice ends. If set to `-1`, will be interpreted as
 * the array object's size (i.e., the total number of elements contained in the array object's internal list).
 * @note This method is very permissive with regards to \p start and \p end . Setting \p start to `-1` will be
 * interpreted as `0`, while setting \p end to `-1` will be interpreted as `n` (where `n` is the number of elements
 * in the array object's internal list). Setting \p start such that it is greater than \p end will simply result
 * in those two values being swapped. If \p start is out of bounds, the array object will simply
 * get cleared up If \p end is out of bounds, the slice will retain every element starting at position \p start .
 * @see fancy_string_array_sliced
 */
void fancy_string_array_slice_and_destroy(fancy_string_array_t *const self, ssize_t start, ssize_t end);

/**
 * @brief Splices the array object's internal list and returns the removed elements (if any) in a new array object.
 * @param self The array object to be spliced.
 * @param index The position at which to splice.
 * @param delete_count The number of items to be removed (and returned) from the array object's internal
 * element list. If set to `0`, no elements will be removed. If set to `-1`, everything starting at position \p index , up
 * to the end, will be removed (and returned).
 * @param optional_new_strings An pointer to an optional \ref fancy_string_array_t instance containing elements to be
 * inserted (in their original order) at position \p index. This argument should be set to the \ref NULL
 * pointer if no elements need to be inserted.
 * @return \ref fancy_string_array_t* A pointer to a new array object containing the removed elements. If no elements
 * were removed, an array object will still be returned, but it will be empty.
 * @note If \p optional_new_strings is non-NULL and non-empty and the insertion position \p index is out of bounds,
 * \p self will be padded with empty string objects up to one less than \p index , and the additional string objects will be
 * added starting at position \p index as requested. If elements are present starting at position \p index , those elements,
 * assuming that \p delete_count is set to `0`, will be moved right by as many positions as there are elements in the
 * \p optional_new_strings array object.
 * @see fancy_string_array_splice_and_destroy fancy_string_array_spliced
 * @par Example:
 * @include examples/fancy_string_array_splice.c
 */
fancy_string_array_t *fancy_string_array_splice(fancy_string_array_t *const self, size_t index, ssize_t delete_count, fancy_string_array_t const *const optional_new_strings);

/**
 * @brief Splices the array object's internal list and destroys the removed elements (if any).
 * @param self The array object to be spliced.
 * @param index The position at which to splice.
 * @param delete_count The number of items to be removed (and destroyed) from the array object's internal
 * element list. If set to `0`, no elements will be removed. If set to `-1`, everything starting at position \p index , up
 * to the end, will be removed (and destroyed).
 * @param optional_new_strings An pointer to an optional \ref fancy_string_array_t instance containing elements to be
 * inserted (in their original order) at position \p index. This argument should be set to the \ref NULL
 * pointer if no elements need to be inserted.
 * @note If \p optional_new_strings is non-NULL and non-empty and the insertion position \p index is out of bounds,
 * \p self will be padded with empty string objects up to one less than \p index , and the additional string objects will be
 * added starting at position \p index as requested. If elements are present starting at position \p index , those elements,
 * assuming that \p delete_count is set to `0`, will be moved right by as many positions as there are elements in the
 * \p optional_new_strings array object. See the documentation for \ref fancy_string_array_splice for an example on splicing.
 * @see fancy_string_array_splice fancy_string_array_spliced
 */
void fancy_string_array_splice_and_destroy(fancy_string_array_t *const self, size_t index, ssize_t delete_count, fancy_string_array_t const *const optional_new_strings);

/**
 * @brief Clones \p self and calls \ref fancy_string_array_splice_and_destroy on that clone.
 * @param self The array object to be cloned and whose clone to be spliced.
 * @param index The position at which to splice.
 * @param delete_count The number of items to be removed (and destroyed) from the clone.
 * If set to `0`, no elements will be removed. If set to `-1`, everything starting at position \p index , up
 * to the end, will be removed (and destroyed).
 * @param optional_new_strings An pointer to an optional \ref fancy_string_array_t instance containing elements to be
 * inserted (in their original order) at position \p index. This argument should be set to the \ref NULL
 * pointer if no elements need to be inserted.
 * @return \ref fancy_string_array_t* A pointer to the cloned and spliced array object.
 * @note If \p optional_new_strings is non-NULL and non-empty and the insertion position \p index is out of bounds,
 * the clone of \p self will be padded with empty string objects up to one less than \p index , and the additional string objects will be
 * added starting at position \p index as requested. If elements are present starting at position \p index , those elements,
 * assuming that \p delete_count is set to `0`, will be moved right by as many positions as there are elements in the
 * \p optional_new_strings array object. See the documentation for \ref fancy_string_array_splice for an example on splicing.
 * @see fancy_string_array_splice_and_destroy fancy_string_array_splice
 */
fancy_string_array_t *fancy_string_array_spliced(fancy_string_array_t const *const self, size_t index, ssize_t delete_count, fancy_string_array_t const *const optional_new_strings);

/**
 * @brief The signature a callback function needs to implement to be allowed to be passed as argument
 * to methods such as \ref fancy_string_array_find_index and \ref fancy_string_array_find .
 * @param string A pointer to the \ref fancy_string_t instance found at position \p index in \p array .
 * @param index The position of \p string in \p array .
 * @param array A pointer to the \ref fancy_string_array_t instance inside which \p string exists.
 * @param context An application-defined context object.
 * @return \ref bool A boolean value indicating whether or not (`true` or `false`) the string object \p string at
 * position \p index in the array object \p array was retained by the implementor of this interface.
 * @see fancy_string_array_find_index, fancy_string_array_find, fancy_string_array_find_last_index, fancy_string_array_find_last,
 * fancy_string_array_some, fancy_string_array_none, fancy_string_array_every
 */
typedef bool (*fancy_string_find_t)(fancy_string_t const *const string, size_t index, fancy_string_array_t const *const array, void *context);

/**
 * @brief Finds the first element in the array object's internal list for which \p fn returns `true` and returns
 * that element's position.
 * @param self A pointer to the \ref fancy_string_array_t instance in which to look for a matching element.
 * @param fn A pointer to a function conforming to the \ref fancy_string_find_t interface, which function gets
 * called (internally) until the first match (i.e., `true` value) is returned.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref ssize_t The position (i.e., index) of the first string object in \p self for which \p fn returned `true`.
 * The value `-1` is returned if all \p fn calls returned `false`, which indicates that not a single match was found.
 * @see fancy_string_find_t, fancy_string_array_find
 * @par Example:
 * @include examples/fancy_string_array_find_index.c
 */
ssize_t fancy_string_array_find_index(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context);

/**
 * @brief Finds the first element in the array object's internal list for which \p fn returns `true` and returns
 * a memory-independent copy of that element.
 * @param self A pointer to the \ref fancy_string_array_t instance in which to look for a matching element.
 * @param fn A pointer to a function conforming to the \ref fancy_string_find_t interface, which function gets
 * called (internally) until the first match (i.e., `true` value) is returned.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref fancy_string_t* A pointer to a memory-independent copy of the first string object in \p self for which \p fn returned `true`.
 * the \ref NULL pointer is returned if all \p fn calls returned `false`, which indicates that not a single match was found.
 * @see fancy_string_find_t, fancy_string_array_find_index
 * @note See the documentation for \ref fancy_string_array_find_index for an example on how to use methods accepting
 * a pointer to a function conforming to \ref fancy_string_find_t for the purpose of identifying elements inside an
 * array object.
 */
fancy_string_t *fancy_string_array_find(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context);

/**
 * @brief Finds the last element in the array object's internal list for which \p fn returns `true` and returns
 * that element's position.
 * @param self A pointer to the \ref fancy_string_array_t instance in which to look for the last matching element.
 * @param fn A pointer to a function conforming to the \ref fancy_string_find_t interface, which function gets
 * called (internally; starting from the last element to the first) until a match (i.e., `true` value) is returned.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref ssize_t The position (i.e., index) of the last string object in \p self for which \p fn returned `true`.
 * The value `-1` is returned if all \p fn calls returned `false`, which indicates that not a single match was found.
 * @see fancy_string_find_t, fancy_string_array_find_last
 * @note See the documentation for \ref fancy_string_array_find_index for an example on how to use methods accepting
 * a pointer to a function conforming to \ref fancy_string_find_t for the purpose of identifying elements inside an
 * array object.
 */
ssize_t fancy_string_array_find_last_index(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context);

/**
 * @brief Finds the last element in the array object's internal list for which \p fn returns `true` and returns
 * a memory-independent copy of that element.
 * @param self A pointer to the \ref fancy_string_array_t instance in which to look for a matching element.
 * @param fn A pointer to a function conforming to the \ref fancy_string_find_t interface, which function gets
 * called (internally; starting from the last element to the first) until a match (i.e., `true` value) is returned.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref fancy_string_t* A pointer to a memory-independent copy of the first string object in \p self for which \p fn returned `true`.
 * the \ref NULL pointer is returned if all \p fn calls returned `false`, which indicates that not a single match was found.
 * @see fancy_string_find_t, fancy_string_array_find_last_index
 * @note See the documentation for \ref fancy_string_array_find_index for an example on how to use methods accepting
 * a pointer to a function conforming to \ref fancy_string_find_t for the purpose of identifying elements inside an
 * array object.
 */
fancy_string_t *fancy_string_array_find_last(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context);

/**
 * @brief Checks whether there is at least one element in the array object's internal list
 * for which \p fn returns `true`.
 * @param self A pointer to the \ref fancy_string_array_t instance in which to check for the
 * presence of at least one matching element.
 * @param fn A pointer to a function conforming to the \ref fancy_string_find_t interface, which function gets
 * called (internally) until a match (i.e., `true` value) is returned.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref bool A boolean value indicating whether or not (i.e., `true` or `false`) at least
 * one string object was found in \p self for which \p fn returned `true`.
 * @see fancy_string_find_t, fancy_string_array_none, fancy_string_array_every
 * @note See the documentation for \ref fancy_string_array_find_index for an example on how to use methods accepting
 * a pointer to a function conforming to \ref fancy_string_find_t for the purpose of identifying elements inside an
 * array object.
 */
bool fancy_string_array_some(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context);

/**
 * @brief Checks whether there are no elements in the array object's internal list
 * for which \p fn returns `true`.
 * @param self A pointer to the \ref fancy_string_array_t instance in which to check for the
 * absence of matching elements.
 * @param fn A pointer to a function conforming to the \ref fancy_string_find_t interface, which function gets
 * called (internally) until a match (i.e., `true` value) is returned. Note that, in this context, a match means
 * that this method will return `false`.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref bool A boolean value indicating whether or not (i.e., `true` or `false`) no
 * string objects were found in \p self for which \p fn returned `true`.
 * @see fancy_string_find_t, fancy_string_array_some, fancy_string_array_every
 * @note See the documentation for \ref fancy_string_array_find_index for an example on how to use methods accepting
 * a pointer to a function conforming to \ref fancy_string_find_t for the purpose of identifying elements inside an
 * array object.
 */
bool fancy_string_array_none(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context);

/**
 * @brief Checks whether \p fn returns `true` for all of the elements in the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance to be checked.
 * @param fn A pointer to a function conforming to the \ref fancy_string_find_t interface, which function gets
 * called (internally) until a non-match (i.e., `false` value) is returned, in which case the method will return
 * `false`, else the function will return `true`, which will mean that all elements are matching.
 * @param context An application-defined context object that gets passed to \p fn on each call.
 * @return \ref bool A boolean value indicating whether or not (i.e., `true` or `false`) \p fn returned `true`
 * for all of the elements in the array object's internal list.
 * @see fancy_string_find_t, fancy_string_array_none, fancy_string_array_some
 * @note See the documentation for \ref fancy_string_array_find_index for an example on how to use methods accepting
 * a pointer to a function conforming to \ref fancy_string_find_t for the purpose of identifying elements inside an
 * array object.
 */
bool fancy_string_array_every(fancy_string_array_t const *const self, fancy_string_find_t fn, void *context);

/**
 * @brief Finds the index of the first element in the array object's internal list whose internal value is equal
 * to \p string 's internal value.
 * @param self A pointer to the \ref fancy_string_array_t instance to be searched for the presence of
 * an element (the first one) whose internal value matches \p string 's internal value.
 * @param string A pointer to the \ref fancy_string_t instance whose internal value is to be looked for
 * in \p self 's internal list.
 * @return \ref ssize_t The position (i.e., the index) of the first matching element in \p self . The value `-1`
 * is returned if no matching element is found.
 * @see fancy_string_array_index_of_value
 * @par Example:
 * @include examples/fancy_string_array_index_of.c
 */
ssize_t fancy_string_array_index_of(fancy_string_array_t const *const self, fancy_string_t const *const string);

/**
 * @brief Finds the index of the first element in the array object's internal list whose internal value is equal
 * to \p value .
 * @param self A pointer to the \ref fancy_string_array_t instance to be searched for the presence of
 * an element (the first one) whose internal value matches \p string 's internal value.
 * @param value A pointer to the value to be searched for in \p self 's internal list.
 * @return \ref ssize_t The position (i.e., the index) of the first matching element in \p self . The value `-1`
 * is returned if no matching element is found.
 * @see fancy_string_array_index_of
 * @note See the documentation for \ref fancy_string_array_index_of for an example on how this method can be
 * used.
 */
ssize_t fancy_string_array_index_of_value(fancy_string_array_t const *const self, char const *const value);

/**
 * @brief Finds the index of the last element in the array object's internal list whose internal value is equal
 * to \p string 's internal value.
 * @param self A pointer to the \ref fancy_string_array_t instance to be searched for the presence of
 * an element (the last one) whose internal value matches \p string 's internal value.
 * @param string A pointer to the \ref fancy_string_t instance whose internal value is to be looked for
 * in \p self 's internal list.
 * @return \ref ssize_t The position (i.e., the index) of the last matching element in \p self . The value `-1`
 * is returned if no matching element is found.
 * @see fancy_string_array_last_index_of_value
 * @note See the documentation for \ref fancy_string_array_index_of for an example on how this method can be
 * used.
 */
ssize_t fancy_string_array_last_index_of(fancy_string_array_t const *const self, fancy_string_t const *const string);

/**
 * @brief Finds the index of the last element in the array object's internal list whose internal value is equal
 * to \p value .
 * @param self A pointer to the \ref fancy_string_array_t instance to be searched for the presence of
 * an element (the last one) whose internal value matches \p string 's internal value.
 * @param value A pointer to the value to be searched for in \p self 's internal list.
 * @return \ref ssize_t The position (i.e., the index) of the last matching element in \p self . The value `-1`
 * is returned if no matching element is found.
 * @see fancy_string_array_last_index_of
 * @note See the documentation for \ref fancy_string_array_index_of for an example on how this method can be
 * used.
 */
ssize_t fancy_string_array_last_index_of_value(fancy_string_array_t const *const self, char const *const value);

/**
 * @brief Checks whether a string object's internal value is contained inside the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance to be searched for the presence of at least
 * one matching element.
 * @param string A pointer to the \ref fancy_string_t instance for whose internal value to search.
 * @return \ref bool A boolean value that indicates whether or not (`true` or `false`) at least one match
 * was found.
 * @see fancy_string_array_includes_value
 * @par Example:
 * @include examples/fancy_string_array_includes.c
 */
bool fancy_string_array_includes(fancy_string_array_t const *const self, fancy_string_t const *const string);

/**
 * @brief Checks whether the value \p value is contained inside the array object's internal list.
 * @param self A pointer to the \ref fancy_string_array_t instance to be searched for the presence of at least
 * one matching element.
 * @param value A pointer to value for which to search.
 * @return \ref bool A boolean value that indicates whether or not (`true` or `false`) at least one match
 * was found.
 * @see fancy_string_array_includes
 * @note See the documentation for \ref fancy_string_array_includes for an example on how this method can
 * be used.
 */
bool fancy_string_array_includes_value(fancy_string_array_t const *const self, char const *const value);

#endif
