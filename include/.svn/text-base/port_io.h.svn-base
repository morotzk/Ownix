/********************************************************/
/* Port I/0 functions to read 1, 2, 4 bytes from ports	*/
/********************************************************/
#ifndef PORT_IO_H
#define PORT_IO_H
#include <sys/types.h>

void outportb(u16_t port, u8_t data);
u8_t inportb(u16_t port);
void outportb_copy(u16_t port, u8_t *buffer, size_t len);
void inportb_copy(u16_t port, u8_t *buffer, size_t len);

void outportw(u16_t port, u16_t data);
u16_t inportw(u16_t port);
void outportw_copy(u16_t port, u16_t *buffer, size_t len);
void inportw_copy(u16_t port, u16_t *buffer, size_t len);

void outportl(u16_t port, u32_t data);
u32_t inportl(u16_t port);
void outportl_copy(u16_t port, u32_t *buffer, size_t len);
void inportl_copy(u16_t port, u32_t *buffer, size_t len);

#endif
