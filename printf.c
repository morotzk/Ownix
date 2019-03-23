#include <printf.h>

static int myisdigit(char c);

void putchar(char c);

/* convert from string to int until first non-digit character!	*/
static int myatoi(char* str);

static void hextoa(unsigned int n, char* buffer) {
	char hex[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	char hex_ch[8], l, h;
	unsigned char *hp;
	int i;
	unsigned int nc;

	nc = n;
	hp = (unsigned char*)&nc;
	for( i = 0; i < 4; ++i) {
		l = *hp & 0xF;
		h = *hp >> 4;
		hex_ch[ 8- (i << 1 + 1) ] = hex[l]; 
		hex_ch[ 8 -((i + 1) << 1) ] = hex[h]; 		
		++hp;
	}

	*buffer++ = '0';
	*buffer++ = 'x';

	for(i = 0; i < 8; ++i) 
		*buffer++ = hex_ch[i];
	
	*buffer = '\0';
}

int printf(const char *format, ...) {
	//Lower case hex digits
	char hex_lc[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};

	//Upper case hex digits
	char hex_uc[] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	char buffer[1024], *bp, *str, c;
	char *fmt;
	int number, quot, rem, nbytes;
	unsigned int addr, uint_number;	

	va_list arg_list;

	va_start(arg_list, format);

	fmt = (char*)format;
	bp = buffer;
	nbytes = 0;
	/* while	*/
	while((c = *fmt++)) {		
		/* switch(c)	*/		
		switch(c) {
				/* case%	*/
				case '%':	{
					/* switch_fmt	*/
					switch(*fmt) {
							/* case_d	*/
							case 'd': {
								number = va_arg(arg_list,int);
								bp = buffer;
								*(bp++)='\0';
								quot = number < 0 ? -number : number;
								if(number < 0) putchar('-');
								if(!number) *(bp++) = '0';
								while(quot >= 1) {
									*(bp++) = (quot % 10)+'0';
									quot /= 10;
								}
								--bp;
								while(*bp) putchar(*bp--), ++nbytes;
								break;
							}
							/* end case_d	*/

						    /* case_u	*/
							case 'u': {
								uint_number = va_arg(arg_list,unsigned int);
								bp = buffer;
								*(bp++) = '\0';
								quot = uint_number;
								if(!quot) *(bp++) = '0';
								while(quot >= 1) *(bp++) = (quot % 10)+'0', quot /= 10;
								--bp;
								while(*bp) putchar(*bp--), ++nbytes;
								break;
							}
							
						    /* case_x	*/
							case 'x': {
								uint_number = va_arg(arg_list, unsigned int);
								bp = buffer;
								*bp++ = '\0';
								quot = uint_number;
								while(quot >= 1) {
									*bp++ = hex_lc[(quot % 0x10)];
									quot /= 0x10;
								}
								--bp;
								while(*bp) putchar(*bp--), ++nbytes;
								break;
							}
						    /* end case_x	*/

						    /* case_p	*/
							case 'p' : {
								uint_number = va_arg(arg_list, unsigned int);
								bp = buffer;
								hextoa(uint_number, bp);
								while(*bp) putchar(*bp++), ++nbytes;
								break;
							}
							/* end case_p	*/

							/* case_X	*/
							case 'X' : {
								uint_number = va_arg(arg_list, unsigned int);
								bp = buffer;
								*bp++ = '\0';
								quot = uint_number;
								while(quot >= 1) {
									*bp++ = hex_uc[(quot % 0x10)];
									quot /= 0x10;
								}
								--bp;
								while(*bp) putchar(*bp--), ++nbytes;
								break;
							}
							/* end case_X */

							/* case_s	*/
							case 's': {
								str = va_arg(arg_list,char*);
								while(*str) putchar(*str++), ++nbytes;
								break;
							}
							/* end case_s	*/
					}
					/* end switch_fmt */

					++fmt;
					break;					

			}
			/* end case%	*/

			default: {
				putchar(c);
				++nbytes;
			}
		}
		/* end switch(c)	*/
	}
	/* end while	*/

	va_end(arg_list);
	
	return nbytes;
}

static int myisdigit(char c) {
	return (c >= '0' && c <= '9');
}

static int myatoi(char* str) {
	char *s;
	int number;

	s = str;
	number = 0;
	while(*s && myisdigit(*s))
		number = number * 10 + (*s++ - '0');

	return number;
}

void putchar(char c) {
	write_screen(c);
	flush();
}
