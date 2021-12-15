#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h> 

char *map_memory_for_read(const char *file_name)
{
  /* Open the bash ELF executable file on Linux */
  int fd = open(file_name, O_RDONLY);
  assert (fd != -1);

  /* Get information about the file, including size */
  struct stat file_info;
  assert (fstat (fd, &file_info) != -1);

  /* Create a private, read-only memory mapping */
  char *mmap_addr = mmap (NULL, file_info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  assert (mmap_addr != MAP_FAILED);

  return mmap_addr;
}

char *map_memory_for_write(const char *file_name, size_t file_size)
{

  /* creating a file with the desired size */
  FILE *out = fopen(file_name, "w");
  for (int i = 0; i < file_size; i++)
  {
    fputc( ' ', out );
  }
	fclose(out);

  /* Open the bash ELF executable file on Linux */
  int fd = open(file_name, O_RDWR, S_IRUSR | S_IWUSR);
  assert (fd != -1);

  /* Get information about the file, including size */
  struct stat file_info;
  assert (fstat (fd, &file_info) != -1);

  /* Create a private, read-only memory mapping */
  char *mmap_addr = mmap (NULL, file_info.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  assert (mmap_addr != MAP_FAILED);

  return mmap_addr;
}
