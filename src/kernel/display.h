#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <types.h>

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

static u8 cursor_x = 0;
static u8 cursor_y = 0;

void setup_text(void);
void clear_text(void);
void write_char(const char c, int pos, char color);
void write_text(const char *text, int pos, char color);
void set_cursor(int x, int y);
void enable_cursor(u8 cursor_start, u8 cursor_end);
void disable_cursor(void);

#endif