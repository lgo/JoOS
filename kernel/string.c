#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#include <string.h>
#include <screen.h>

size_t strlen(const char* str)
{
  size_t ret = 0;
  while ( str[ret] != 0 )
    ret++;
  return ret;
}

void print(const char* str)
{
  terminal_writestring(str);
}
