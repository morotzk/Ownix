#include <fs/path_operations.h>

int get_next_dir_in_path(char* path, char* next_dir) {
	int len = 0;
	char* p = path;
	char* dir = next_dir;
	
	if(*p == '/') ++p;
	while(*p != '\0' && *p != '/') {
		if(len == MAX_PATH_LENGTH - 1) {
			*next_dir = '\0';
			return EPATHLONG;
		}				
		
		*next_dir++ = *p++;		
		++len;
	}
	*next_dir = '\0';
	if(*p == '\0')
		return ENODIR;
	
	return 0;	
}

char* skip_chars_while_equals(char* path, char* pattern) {
	char* p = path;
	char* pn = pattern;
	
	if(*p == '/') ++p;
	while(*p == *pn) ++p, ++pn;
	return p;
}

