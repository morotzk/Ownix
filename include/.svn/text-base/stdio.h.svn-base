#ifndef STDIO_H
#define STDIO_H

#define BUFSIZ 		2048
#define FILENAME_MAX	256
#define FOPEN_MAX	16

#define _IOFBF 		1
#define _IOLBL		2
#define _IONBF		4

#define SEEK_CUR	1
#define SEEK_END	2
#define SEEK_SET	4

#define EOF		-1
#define NULL		((void*)0)

/* 	From MiniX 3		*/
typedef struct iobuf {
	int		count;
	int		fd;
	int		flags;
	int		bufsiz;
	unsigned char	*buf;
	unsigned char	*ptr;
} FILE;

int printf(const char *, ...);
int putchar(int);
int getchar(void);
void perror(const char *);
int puts(const char *);
char *gets(char *);

/*------------------------------*/
/*	Not yet implemented	*/
/*------------------------------*/

/*
int fclose(FILE*);
FILE* fdopen(int, const char*);
int feof(FILE*);
int ferror(FILE*);
int fflush(FILE*);
int fgetc(FILE*);
int fgetpos(FILE *, fpos_t *);
char* fgets(char *, int, FILE *);
int fileno(FILE *);
void flockfile(FILE *);
FILE* fopen(const char *, const char *);
int fprintf(FILE *, const char *, ...);
int fputc(int, FILE *);
int fputs(const char *, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
FILE* freopen(const char *, const char *, FILE *);
int fscanf(FILE *, const char *, ...);
int fseek(FILE *, long int, int);
int fseeko(FILE *, off_t, int);
int fsetpos(FILE *, const fpos_t *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int getc(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
char *gets(char *);
int getw(FILE *);
int pclose(FILE *);
FILE *popen(const char *, const char *);
int putc(int, FILE *);
int putchar(int);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);
int puts(const char *);
int putw(int, FILE *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int scanf(const char *, ...);
void setbuf(FILE *, char *);
int setvbuf(FILE *, char *, int, size_t);
int snprintf(char *, size_t, const char *, ...);
int sprintf(char *, const char *, ...);
int sscanf(const char *, const char *, int ...);
char * tempnam(const char *, const char *);
FILE* tmpfile(void);
char* tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE *, const char *, va_list);
int vprintf(const char *, va_list);
int vsnprintf(char *, size_t, const char *, va_list);
int vsprintf(char *, const char *, va_list);
*/

#endif
