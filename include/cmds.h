extern unsigned char __bss_end;
extern unsigned char __bss_start;


void clear_bss();
void hexdump(unsigned char *buffer, unsigned int length);
unsigned int getEL();