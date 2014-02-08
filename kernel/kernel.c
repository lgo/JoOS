#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>
#include <screen.h>
 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif
 
/* kernel entry point */
void kernel_main()
{
  init_screen();
  
  terminal_writestring("Hello, kernel's World!\nSss");
  terminal_writestring("Even more\n\n\n hmm \n\n\n\n\n\n\n yes...\n\n\n\n\n\n\n\n\nMORE\nHmm\nYes :)\nMUAHAHA\n\n\n\n WEEE SKIP LINE");
}
