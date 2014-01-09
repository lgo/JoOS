#if !defined(SCREEN_H)
#define SCREEN_H

void terminal_writestring(const char* data);
void terminal_rewriteprev(const char* data);
void terminal_rewriteago(const char* data, size_t lines);

void init_screen();

#endif

