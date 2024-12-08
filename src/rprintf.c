/*------------------------------------------------------------------------------*/
/* Public Domain version of printf - MODIFIED BY SEAVER OLSON                   */
/*------------------------------------------------------------------------------*/

#include "rprintf.h"

static func_ptr out_char;
static int do_padding;
static int left_flag;
static int len;
static int num1;
static int num2;
static char pad_character;
#define UART_LSR_RXFE (1 << 0)
volatile int *muio_ptr = (volatile int *)(AUX_MU_IO);

void putc(int data){
   if (muio_ptr != NULL) *muio_ptr = data;
}

/*
char getc(){
	while (!(*AUX_MU_LSR&0x01)) asm volatile("nop");
	char c = (char)(*AUX_MU_IO);
	if (c=='\r') return '\n'; 
   return c;
}
*/
char getc(){
 	while (!(*AUX_MU_LSR & 0x01)) {
		asm volatile("nop");
	}
        esp_printf(putc, "data available");
	return (char)(*AUX_MU_LSR & 0xFF);
}
char getc_NB(){
    if (*AUX_MU_LSR & 0x01){
        char received_char = (char)(*AUX_MU_IO & 0xFF);  // Read the received byte
        esp_printf(putc, "Received character: 0x%02X\n", received_char);  // Debug output
        return received_char;
    }
    return 0;
}

void logc(int data){
   //send data to log.txt

}

size_t strlen(const char *str) {
    unsigned int len = 0;
    while(str[len] != '\0') len++;
    return len;
}

int tolower(int c) {
    if(c < 'a') c -= 'a' - 'A';
    return c;
}

int isdig(int c) {
    if((c >= '0') && (c <= '9')) return 1;
    else return 0;
}


int strcmp(char *first, char *second){
	while(*first &&(*first==*second)){
		first++;
		second++;
	}
	return *(unsigned char *)first - *(unsigned char *)second;
}

void *memcpy(void *dest, const void *src, size_t n){
	if (dest == NULL || src == NULL)return NULL;
	if (n==0)return dest;
        unsigned char *d = (unsigned char *)dest;
	const unsigned char *s = (const unsigned char *)src;
	while (n--){
		*d++ = *s++;
	}
	return dest;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

int strncmp(char *first, char *second, size_t n){
	while (n && *first && (*first == *second)){
		first++;
		second++;
		n--;
	}
	if (n == 0){
		return 0;
	}
	return *(unsigned char *)first - *(unsigned char *)second;
}


/*---------------------------------------------------*/
/*                                                   */
/* This routine puts pad characters into the output  */
/* buffer.                                           */
/*                                                   */
static void padding( const int l_flag)
{
   int i;

   if (do_padding && l_flag && (len < num1))
      for (i=len; i<num1; i++)
          out_char( pad_character);
   }

/*---------------------------------------------------*/
/*                                                   */
/* This routine moves a string to the output buffer  */
/* as directed by the padding and positioning flags. */
/*                                                   */
static void outs( charptr lp)
{
   if(lp == NULL)
      lp = "(null)";
   /* pad on left if needed                          */
   len = strlen( lp);
   padding( !left_flag);

   /* Move string to the buffer                      */
   while (*lp && num2--)
      out_char( *lp++);

   /* Pad on right if needed                         */
   len = strlen( lp);
   padding( left_flag);
   }

/*---------------------------------------------------*/
/*                                                   */
/* This routine moves a number to the output buffer  */
/* as directed by the padding and positioning flags. */
/*                                                   */
static void outnum(unsigned int num, const int base)
{
    char outbuf[32];
    const char digits[] = "0123456789ABCDEF";
    int negative = 0;
    char *cp = outbuf;

    if (num < 0L && base != 16L) {
        negative = 1;
        num = -num;
    }

    // Build number (backwards) in outbuf
    do {
        *cp++ = digits[num % base];
    } while ((num /= base) > 0);

    if (negative) *cp++ = '-';
    *cp = '\0'; 

    len = strlen(outbuf);
    padding(!left_flag);
    while (--cp >= outbuf) {
        out_char(*cp);
    }
    padding(left_flag);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine gets a number from the format        */
/* string.                                           */
/*                                                   */
static int getnum( charptr* linep)
{
   int n;
   charptr cp;

   n = 0;
   cp = *linep;
   while (isdig((int)*cp))
      n = n*10 + ((*cp++) - '0');
   *linep = cp;
   return(n);
}

/*---------------------------------------------------*/
/*                                                   */
/* This routine operates just like a printf/sprintf  */
/* routine. It outputs a set of data under the       */
/* control of a formatting string. Not all of the    */
/* standard C format control are supported. The ones */
/* provided are primarily those needed for embedded  */
/* systems work. Primarily the floaing point         */
/* routines are omitted. Other formats could be      */
/* added easily by following the examples shown for  */
/* the supported formats.                            */
/*                                                   */

void esp_printf( const func_ptr f_ptr, charptr ctrl, ...)
{
  va_list args;
  va_start(args, ctrl);
  esp_vprintf(f_ptr, ctrl, args);
  va_end( args );
  
}
void esp_vprintf(const func_ptr f_ptr, charptr ctrl, va_list argp) {
    int long_flag, dot_flag, left_flag, do_padding;
    char ch;
    //char pad_character = ' ';
    //int num1 = 0, num2 = 32767; // Default values, can be customized
    out_char = f_ptr; // Set output function

    for (; *ctrl; ctrl++) {
        if (*ctrl != '%') {
            out_char(*ctrl); // Print regular character
            continue;
        }

        // Initialize flags
        long_flag = dot_flag = left_flag = do_padding = 0;
        pad_character = ' ';

        // Move to next character (after '%')
        ch = *(++ctrl);

        // Handle numeric specifiers (width)
        if (isdig((int)ch)) {
            if (dot_flag) {
                num2 = getnum(&ctrl);
            } else {
                if (ch == '0') pad_character = '0';
                num1 = getnum(&ctrl);
                do_padding = 1;
            }
            ctrl--; // Correctly adjust for the loop increment
            continue;
        }

        // Handle format specifiers
        switch (tolower((int)ch)) {
            case '%':
                out_char('%');
                break;

            case '-':
                left_flag = 1;
                break;

            case '.':
                dot_flag = 1;
                break;

            case 'l':
                long_flag = 1;
                break;

            case 'i': // Signed integer
            case 'd': 
                if (long_flag || ch == 'D') {
                    outnum(va_arg(argp, long), 10L); // Print long
                } else {
                    outnum(va_arg(argp, int), 10L); // Print int
                }
                break;

            case 'u': // Unsigned integer
                if (long_flag) {
                    outnum(va_arg(argp, unsigned long), 10L); // Print unsigned long
                } else {
                    outnum(va_arg(argp, unsigned int), 10L); // Print unsigned int
                }
                break;

            case 'x': // Hexadecimal (unsigned)
                outnum((long)va_arg(argp, unsigned int), 16L); // Print unsigned int as hex
                break;

            case 's':
                outs(va_arg(argp, charptr)); // Print string
                break;

            case 'c':
                out_char(va_arg(argp, int)); // Print character
                break;

            case '\\':
                switch (*ctrl) {
                    case 'a': out_char(0x07); break;
                    case 'h': out_char(0x08); break;
                    case 'r': out_char(0x0D); break;
                    case 'n': out_char(0x0D); out_char(0x0A); break;
                    default: out_char(*ctrl); break;
                }
                ctrl++;
                break;

            default:
                esp_printf(f_ptr, "Error: Unsupported format specifier %c\n", ch);
                break;
        }
    }
}


void esp_printhex(unsigned int num){
   esp_printf(putc, "0x%08X\n", num);
}
/*---------------------------------------------------*/
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

void success(char *str){
   green();
   esp_printf(putc, "%s\n", str);
   resetColor();
}

void fail(char *str){
   red();
   esp_printf(putc, "%s\n", str);
   resetColor();
}

void warning(char *str){
   yellow();
   esp_printf(putc, "%s\n", str);
   resetColor();
}


void debug(char *str){
   //send to debug port
   esp_printf(logc, "%s\n", str);
}

void logo(){
    blue();
    esp_printf(putc, "\n\n");
    esp_printf(putc, " ____  ____  ____  _  _  _____  ___ \n");
    esp_printf(putc, "(  _ \\( ___)( ___)( \\( )(  _  )/ __)\n");
    esp_printf(putc, " )___/ )__)  )__)  )  (  )(_)( \\__ \\\n");
    esp_printf(putc, "(__)  (____)(____)(_\\_)(_____)(___/\n");
    esp_printf(putc, "\n\n");
    resetColor();
}

