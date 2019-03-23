#include <port_io.h>

void outportb(u16_t port, u8_t data) {
	asm volatile("outb %1, %0" : : "dN" (port), "a"(data));
}

u8_t inportb(u16_t port) {
	u8_t data;
	
	asm volatile("inb %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

void outportb_copy(u16_t port, u8_t *buffer, size_t len) {
	u32_t i;
	u8_t data;
	
	for(i = 0; i < len; ++i) {
		data = *(buffer++);
		outportb(port, data);
	}
}

void inportb_copy(u16_t port, u8_t *buffer, size_t len) {
	u32_t i;
	u8_t data;
	
	for(i = 0; i < len; ++i) {
		data = inportb(port);
		*(buffer++) = data;
	}
}

void outportw(u16_t port, u16_t data) {
	asm volatile("outw %1, %0" : : "dN" (port), "a"(data) );
}

u16_t inportw(u16_t port) {
	u16_t data;
	
	asm volatile("inw %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

void outportw_copy(u16_t port, u16_t *buffer, size_t len) {
	u32_t i;
	u16_t data;
	
	for(i = 0; i < len; ++i) {
		data = *(buffer++);
		outportw(port, data);
	}
}

void inportw_copy(u16_t port, u16_t *buffer, size_t len) {
	u32_t i;
	u16_t data;
	
	for(i = 0; i < len; ++i) {
		data = inportw(port);
		*(buffer++) = data;
	}
}

void outportl(u16_t port, u32_t data) {
	asm volatile("outl %1, %0" : : "dN" (port), "a"(data) );	
}

u32_t inportl(u16_t port) {
	u32_t data;
	
	asm volatile("inl %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

void outportl_copy(u16_t port, u32_t *buffer, size_t len) {
	u32_t i;
	u32_t data;
	
	for(i = 0; i < len; ++i) {
		data = *(buffer++);
		outportl(port, data);
	}
}

void inportl_copy(u16_t port, u32_t *buffer, size_t len) {
	u32_t i;
	u16_t data;
	
	for(i = 0; i < len; ++i) {
		data = inportl(port);
		*(buffer++) = data;
		
	}
}

