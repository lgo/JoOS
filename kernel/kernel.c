#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This kernel will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif
 
/* Hardware text mode color constants. */
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

/* Combining colors */
uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
 
/* Write to the VGA memory*/
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
/* Return the length of a string */
size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
uint16_t empty;

/* init kernal terminal */
void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
  empty = make_vgaentry(' ', terminal_color);
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = empty;
		}
	}
}

/* set kernal background color */
void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

/* init the line  */
void terminal_initline(size_t line)
{
  for (size_t x = 0; x < VGA_WIDTH; x++)
  {
    const size_t index = line * VGA_WIDTH + x;
    terminal_buffer[index] = empty;
  }
}

/* move all of the characters in the buffer upwards  */
void terminal_scrollup()
{
  for (size_t y = 1; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t index = y * VGA_WIDTH + x;
      const size_t new_index = index - VGA_WIDTH;
      terminal_buffer[new_index] = terminal_buffer[index];
    }
  }
  terminal_initline(VGA_WIDTH);
}

/* reposition the writing placement */
void terminal_resetline()
{
  if (++terminal_column == VGA_WIDTH)
  {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
    {
      terminal_scrollup();
      --terminal_row;
    }
  }
} 

/* push a character and color to screen grid location x,y */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

/* push a character to the screen */
void terminal_putchar(char c)
{
  terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
  terminal_resetline();
}

/* move the position of the writer to a new line  */
void terminal_newline()
{
    terminal_column = VGA_WIDTH - 1; //bit of a hack to get everything to cooperate the way it is
    terminal_resetline();
}

/* write a string to the terminal */
void terminal_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
        {
		if (data[i] == '\n')
                {
                  terminal_newline();
                } else
                {
                  terminal_putchar(data[i]);
                }
        }
}

/* kernel entry point */
void kernel_main()
{
	terminal_initialize();
	// our little bit of hello world!
        terminal_writestring("Hello, kernel's World!\nSss");
        terminal_writestring("Even more\n\n\n hmm \n\n\n\n\n\n\n yes...\n\n\n\n\n\n\n\n\nMORE\n\n\n\nHmm\nYes :)");
}
