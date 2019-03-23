#include <lib/string.h>

char* strcpy(char* dest, const char* src) {
	char* s;
	char* d;

	for(s = (char*) src, d = dest; *s; *d = *s, ++s, ++d);
	*d = '\0';
	return dest;
}

char* strncpy(char* dest, const char* src, size_t n) {
        size_t i;

        for (i = 0 ; i < n && src[i] != '\0' ; i++)
        	dest[i] = src[i];
        for ( ; i < n ; i++)
        	dest[i] = '\0';

        return dest;
}

int strcmp(const char *s, const char *d) {	
	for( ; *s && *d; ++s, ++d) if(*s != *d) return -1;			
	/* first string is bigger than second 	*/
	if(*s != '\0') return 1;		
	/* first string is smaller than second	*/
	else if(*d != '\0') return -2;
	
	return 0;
}

int strncmp(char* s, char* d, size_t n) {
	int i;
	
	for(i = 0; i < n && s[i] != '\0' && d[i] != '\0'; ++i) {
		if(s[i] != d[i])
			return -1;
	}
	
	if(s[i] != '\0' || d[i] != '\0')
		return -2;
		
	return 0;
}

char* strcat(char* dest, const char* src) {
	char* cnct = dest;

	while(*dest++ != '\0') ;
	--dest;
	while(*src != '\0')
		*dest++ = *src++;
	*dest = '\0';
	
	return cnct;
}

size_t strlen(char* s) {
	size_t size = 0;
	
	while(*s++ != '\0') ++size;
	return size;
}

