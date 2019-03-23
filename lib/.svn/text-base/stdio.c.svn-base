#include <stdio.h>

/* extern allow us to implement it as a syscall later!	
 * And internally we use our kernel putc!
*/
extern void putc(char c);

int printf(const char *format, ...) {
	char buf[1024], *fmt;
	
	fmt = format;
	while(fmt++) {
		
	}
}

int putchar(int c) {
	putc(c);
	return 0;
}

int getchar(void) {
	
}

void perror(const char *str) {
	puts("error : ");
	puts(str);
	putc('\n');
}

int puts(const char *str) {
	int nbytes;
	
	nbytes = 0;
	while(*str++) {
		putc(*str);
		++nbytes;
	}
	
	return nbytes;
}

char *gets(char *) {

}
