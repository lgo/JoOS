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

// Compare two strings. Returns 0 if equal, else 1.
int strcmp(char * str1, char * str2)
{
  int i =0;
  int failed = 0;
  while (str1[i] != '\0' && str2[i] != '\0')
  {
    if (str1[i] != str2[i])
    {
      failed = 1;
      break;
    }
    i++;
  }
  if (str1[i] == '\0' || str2[i] == '\0')
  {
    failed = 1;
  }

  return failed;
  
}

// Copy null-terminated string from src into dest, return dest.
char * strcpy(char * dest, const char * src)
{
  do
  {
    *dest = *src;
    dest++;
    src++;
  } while (*src != '\0');
  return dest;
}

// Concat src to the end of dest
char * strcat (char * dest, const char *src)
{
  char * tmp = dest;
  while (*dest != '\0') dest++;
  do
  {
    *dest = *src;
    dest++;
    src++;
  } while (*src != '\0');

  return tmp;
}

void print(const char* str)
{
  terminal_writestring(str);
}
