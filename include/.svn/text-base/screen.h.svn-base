#ifndef SCREEN_H
#define SCREEN_H
#include <sys/types.h>
#include <mem_util.h>
#include <protect.h>

#define SCREEN_ROWS		25
#define SCREEN_COLS		80
#define SCREEN_SIZE		(SCREEN_ROWS * SCREEN_COLS)
#define MAX_SCREEN_POS		(SCREEN_SIZE - 1)
#define SCREEN_BUF_LEN		SCREEN_COLS
#define TAB_WIDTH		8
#define TAB_MASK		0xFFF8

#define VIDEO_MEM_START		0xB8000
#define DEFAULT_ATTR		0xF8 //0x78

#define DEFAULT_BG_ATTR		0xF0

/* Color constants					*/
#define COLOR_BLACK		0x0
#define COLOR_BLUE		0x1
#define COLOR_GREEN		0x2
#define COLOR_CYAN		0x3
#define COLOR_RED		0x4
#define COLOR_MAGENTA		0x5
#define COLOR_BROWN		0x6
#define COLOR_LIGHTGREY		0x7
#define COLOR_DARKGREY		0x8
#define COLOR_BRIGHTBLUE	0x9
#define COLOR_BRIGHTGREEN	0xA
#define COLOR_BRIGHTCYAN	0xB
#define COLOR_BRIGHTRED		0xC
#define COLOR_BRIGHTMAGENTA	0xD

typedef u16_t screen_char_t;
typedef u8_t  screen_attr_t;

struct screen_line_buf {
	screen_char_t screen_buffer[SCREEN_BUF_LEN];
	u8_t current_len; 
};

#define SCREEN_CHAR(c, a) 	((screen_char_t)c << 8 | a)
#define CHAR_PART(sc)		((u8_t)sc >> 8)
#define CURRENT_POS(row,col) 	((u16_t)row  * SCREEN_COLS + col)
#define BLANK			SCREEN_CHAR(' ',attr)
/* Color taken from the constants above!		*/
#define NEW_ATTR(color)		(DEFAULT_BG_ATTR | color)

/* flush the contents of the screen_buffer to screen	*/
void flush();

/* write char to the buffer				*/
void write_screen(u8_t c);

/* init screen variables				*/
void init_screen();

/* Clear the screen					*/
void clear_screen();

/* Set the current screen attribute			*/
void set_attribute(screen_attr_t attribute);

/* Get the current screen attribute			*/
screen_attr_t get_attribute();

#endif
