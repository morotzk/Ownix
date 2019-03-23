#include <screen.h>

screen_char_t *screen;
static u16_t row;
static u16_t column;
static screen_attr_t attr;
static struct screen_line_buf sbuf;

/* move the cursor to the current position		*/
static void move_cursor();
/* Scroll the screen					*/
static void scroll();
/* Put a character direct to screen			*/
static void put(screen_char_t sc);
/* Function to clear a line				*/
static void clear_line(u16_t line);
/********************************************************
 *			Implementation			*
 *******************************************************/
 void flush() {
 	u8_t i, len;
 
 	if(sbuf.current_len > 0) {
 		len = sbuf.current_len;
 		for(i = 0; i < len; ++i, --sbuf.current_len) {
 			put(sbuf.screen_buffer[i]);
 		}	
 	}
 }
 
 void write_screen(u8_t c) {
 	if(sbuf.current_len < SCREEN_BUF_LEN) {
 		if( c == '\n' ) {
 			sbuf.screen_buffer[sbuf.current_len++] = SCREEN_CHAR(attr,c);
 			flush();	
 		} else {
 			sbuf.screen_buffer[sbuf.current_len++] = SCREEN_CHAR(attr,c);	
 		} 
 	} else {
 		flush();
 		sbuf.screen_buffer[sbuf.current_len++] = SCREEN_CHAR(attr,c);
 	} 
 }
 
 void init_screen() {
 	screen = (screen_char_t*)VIDEO_MEM_START;
 
 	clear_screen();
 
 	row = column = 0;
 	attr = DEFAULT_ATTR;
 	
 	sbuf.current_len = 0;
 	memset(sbuf.screen_buffer, 0, SCREEN_BUF_LEN * sizeof(screen_char_t));
 	
 }
 
 void clear_screen() {
 	u16_t blank;
 	blank = (DEFAULT_ATTR << 8) + 0x20;
 	memsetw((void*)screen, blank, SCREEN_SIZE);
 	row = column = 0;
 	move_cursor();
 }
 
 static void move_cursor() {
 	outportb(0x3D4, 14);
 	outportb(0x3D5, CURRENT_POS(row,column) >> 8);
 	outportb(0x3D4, 15);
 	outportb(0x3D5, CURRENT_POS(row,column));
 }
 
 static void scroll() {
 	u16_t i, blank;
 	u32_t first_row, last_row;

 	blank = (DEFAULT_ATTR << 8) + 0x20;
 	
 	first_row = (u32_t)screen + (1 * SCREEN_COLS * sizeof(screen_char_t));
 	memcpyw((void*)first_row, (void*)screen, (SCREEN_ROWS - 1) * SCREEN_COLS);
	
 	/* Clear the last line , fill it with blank	*/	
 	clear_line(SCREEN_ROWS - 1);
 }
 
 void set_attribute(screen_attr_t attribute) {
 	attr = attribute;
 }
 
 screen_attr_t get_attribute() {
 	return attr;
 }
 
 static void put(screen_char_t sc) {
 	u8_t ch;
 	
 	ch = (u8_t)sc & 0xFF;
 	switch(ch) {
 		case '\n': {
 			row += 1;
 			column = 0;
 			goto need_scroll;
 			break;
 		}
 			
 		case '\t': {
 		
 			if(((column + TAB_WIDTH) & TAB_MASK) > SCREEN_COLS) {
 				column = 0;
 				row += 1;
 			} else {
 				/* Align to 8-dividable position	*/
 				column = (column + TAB_WIDTH) & TAB_MASK;
 			}	
 			move_cursor();
 			goto need_scroll;
 			break; 			
 		}	
 	}
 	
 	print: screen[CURRENT_POS(row,column)] = sc;
 	       move_cursor();
 	       column += 1;
 	       if(column >= SCREEN_COLS) row += 1, column = 0;	
 	
 	need_scroll: if(CURRENT_POS(row,column) > MAX_SCREEN_POS) {
 			scroll();
 			column = 0;
 			row = SCREEN_ROWS - 1;
 			move_cursor();
 		     } else {
 		     	move_cursor();
 		     }
 }

 static void clear_line(u16_t line) {
 	u16_t blank;
 	u32_t row_start_addr;
 
 	if(line >= SCREEN_ROWS) return; 	
 	
 	blank = (DEFAULT_ATTR << 8) + 0x20;
 	row_start_addr = (u32_t)screen + (line * SCREEN_COLS * sizeof(screen_char_t));
 	memsetw((void*)row_start_addr, blank, SCREEN_COLS);
 }
 
