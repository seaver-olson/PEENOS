#ifndef __RPRINTF_H__
#define __RPRINTF_H__

//#include <ctype.h>
//#include <string.h>
#include <stdarg.h>
#include "gpio.h"
#include "delays.h"
typedef unsigned int  size_t;

#define NULL (void*)0


#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define CLEAR "\033[0;0m"
#define BLACK "\033[0;30m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"


void putc(int data);
char getc();
char getc_NB();
int strcmp(char *first, char *second);
void *memcpy(void *dest, const void *src, size_t n);
int strncmp(char *first, char *second, size_t n);
size_t strlen(const char *str);
void *memset(void *s, int c, size_t n);
int tolower(int c);
int isdig(int c);

int isdig(int c); // hand-implemented alternative to isdigit(), which uses a bunch of c library functions I don't want to include.

typedef char *charptr;
typedef void (*func_ptr)(int);


void esp_sprintf(char *buf, char *ctrl, ...);
void esp_vprintf( const func_ptr f_ptr, charptr ctrl, va_list argp);
void esp_printf( const func_ptr f_ptr, charptr ctrl, ...);
void esp_printhex(unsigned int num);
void printk(charptr ctrl, ...);


void red();
void green();
void yellow();
void blue();
void cyan();
void resetColor();
void purple();
void black();
void white();

void success(char *str);
void fail(char *str);
void warning(char *str);




#endif
