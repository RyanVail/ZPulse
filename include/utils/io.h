#ifndef IO_H
#define IO_H

#include <common.h>

/**
 * Attempts to read the entire contents of a file.
 *
 * @param filename The path (relative or absolute) to the file.
 * @param len A pointer to a value that will be filled with the byte size of
 * the file.
 *
 * @return The contents of the file on success, `NULL` on failure.
 */
u8* io_read_file(const char* filename, size_t* len);

#endif
