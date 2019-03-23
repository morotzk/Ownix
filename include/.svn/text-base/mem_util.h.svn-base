#ifndef MEM_UTIL_H
#define MEM_UTIL_H
#include <sys/types.h>

/* Copy len bytes from src to dst		*/
void memcpy(void* src, void* dst, size_t len);

/* Copy len words from src to dst		*/
void memcpyw(void* src, void* dst, size_t len);

/* Copy len doublewords from src to dst		*/
void memcpyl(void* src, void* dst, size_t len);

/* Fill mem witch c in bytes steps		*/
void memset(void* dst, u8_t c, size_t len);

/* Fill mem with w in words steps		*/
void memsetw(void* dst, u16_t w, size_t len);

/* Fill mem with l in dowublewords steps	*/
void memsetl(void* dst, u32_t l, size_t len);

/* Macro to extract the nth byte from a doubleword(n = 0..3!)	*/
#define extract_n_th_byte(dw, n) ((dw >> n * 8) & 0xff)

/* Conversion from lsb to msb	*/
#define lsb_to_msb(n,res) 		\
res = extract_n_th_byte(n,3) | 		\
      extract_n_th_byte(n,2) << 8 | 	\
      extract_n_th_byte(n,1) << 16 | 	\
      extract_n_th_byte(n,0) <<24;

#endif
