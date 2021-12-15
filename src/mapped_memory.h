#ifndef MAPPED_MEMORY_H
#define MAPPED_MEMORY_H

#include <stdlib.h>

char *map_memory_for_read(const char *file_name);
char *map_memory_for_write(const char *file_name, size_t file_size);

#endif // MAPPED_MEMORY_H
