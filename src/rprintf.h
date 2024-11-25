#ifndef __RPRINTF_H__
#define __RPRINTF_H__

//#include <ctype.h>
//#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "gpio.h"

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
int strcmp(char *first, char *second);
void *memcpy(void *dest, const void *src, size_t n);
int strncmp(char *first, char *second, size_t n);
size_t strlen(const char *str);

int isdig(int c); // hand-implemented alternative to isdigit(), which uses a bunch of c library functions I don't want to include.

typedef char *charptr;
typedef void (*func_ptr)(int);

///////////////////////////////////////////////////////////////////////////////
////  Common Prototype functions
/////////////////////////////////////////////////////////////////////////////////
void esp_sprintf(char *buf, char *ctrl, ...);
void esp_vprintf( const func_ptr f_ptr, charptr ctrl, va_list argp);
void esp_printf( const func_ptr f_ptr, charptr ctrl, ...);
void esp_printhex(unsigned int num);
void printk(charptr ctrl, ...);
size_t strlen(const char *c);
int tolower(int c);
void success(char *str);
void fail(char *str);
void warning(char *str);

void red(){
	esp_printf(putc, RED);
}

void green(){
	esp_printf(putc, GREEN);
}

void yellow(){
	esp_printf(putc, YELLOW);
}

void blue(){
	esp_printf(putc, BLUE);
}

void cyan(){
	esp_printf(putc, CYAN);
}

void resetColor(){
	esp_printf(putc, CLEAR);
}

void purple(){
	esp_printf(putc, PURPLE);
}

void black(){
	esp_printf(putc, BLACK);
}

void white(){
	esp_printf(putc, WHITE);
}

#endif
