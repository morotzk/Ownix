#include <mem_util.h>

void memcpy(void* src, void* dst, size_t len) {
	u8_t *s, *d;
	int i;
	
	for(i = 0, s = (u8_t*)src, d = (u8_t*)dst; i < len; *d = *s, ++s, ++d, ++i) ;
}

void memcpyw(void* src, void* dst, size_t len) {
	u16_t *s, *d;
	int i;
	
	for(i = 0, s = (u16_t*)src, d = (u16_t*)dst; i < len; *d = *s, ++s, ++d, ++i) ;
}

void memcpyl(void* src, void* dst, size_t len) {
	u32_t *s, *d;
	int i;
	
	for(i = 0, s = (u32_t*)src, d = (u32_t*)dst; i < len; *d = *s, ++s, ++d, ++i) ;
}

void memset(void* dst, u8_t c, size_t len) {
	u8_t *d;
	int i;
	
	for(i = 0, d = (u8_t*)dst; i < len; *d = c, ++d, ++i);
}

void memsetw(void* dst, u16_t w, size_t len) {
	u16_t *d;
	int i;
	
	for(i = 0, d = (u16_t*)dst; i < len; *d = w, ++d, ++i);	
}

void memsetl(void* dst, u32_t l, size_t len) {
	u32_t *d;
	int i;
	
	for(i = 0, d = (u32_t*)dst; i < len; *d = l, ++d, ++i);	
}

